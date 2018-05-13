#include "DiffuseNormalShader.h"


DiffuseNormalShader::DiffuseNormalShader(WCHAR* vsFilenPath, WCHAR* psFilenPath):Shader(vsFilenPath, psFilenPath)
{
	CreateBuffer();
}


DiffuseNormalShader::DiffuseNormalShader(const DiffuseNormalShader& other) :Shader(other)
{

}


DiffuseNormalShader::~DiffuseNormalShader()
{
	ShutDown();
}

void DiffuseNormalShader::ShutDown()
{
	ReleaseCOM(mCBLightBuffer)
}

void DiffuseNormalShader::CreateBuffer()
{
	mCBLightBuffer = NULL;

	ID3D11Device* d3dDevice = D3DClass::GetInstance()->GetDevice();

	D3D11_BUFFER_DESC lightBufferDesc;
	ZeroMemory(&lightBufferDesc, sizeof(lightBufferDesc));
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(CBLight);   //�ṹ���С,����Ϊ16�ֽڱ���
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	d3dDevice->CreateBuffer(&lightBufferDesc, NULL, &mCBLightBuffer);
}

bool DiffuseNormalShader::SetShaderCBExtern(CXMMATRIX worldMatrix, ID3D11ShaderResourceView* diffuseSRV, ID3D11ShaderResourceView* normalSRV)
{
	ID3D11DeviceContext* d3dDeviceContext = D3DClass::GetInstance()->GetDeviceContext();
	Shader::SetShaderCB(worldMatrix);



	d3dDeviceContext->PSSetShaderResources(0, 1, &diffuseSRV);
	d3dDeviceContext->PSSetShaderResources(1, 1, &normalSRV);
	return true;
}


bool DiffuseNormalShader::SetShaderParamsExtern(CXMMATRIX worldMatrix,
	ID3D11ShaderResourceView* diffuseSRV, ID3D11ShaderResourceView* normalSRV)
{

	bool result;

	//����Shader���������������Դ
	result = SetShaderCBExtern(worldMatrix, diffuseSRV,normalSRV);
	if (!result)
		return false;

	//����VertexShader PixelShader InputLayout SamplerState
	SetShaderState();

	return true;
}