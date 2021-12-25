#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "winsock2.h"
#include "stdio.h"

#pragma comment(lib,"ws2_32.lib")


void _ERROR(const char* s)
{
	printf("%s 出错！出错代码：%d \n", s, WSAGetLastError());
}

int count(char* s)
{
	int cc = 0;
	for (int i = 0; s[i] != 0; i++)
	{
		if (isalpha(s[i]))
		{
			cc++;
		}
	}
	return cc;
}


int main(void)
{

	WSADATA wsaData;
	if (WSAStartup(0x0202, &wsaData) != 0)
	{
		_ERROR("WSAStartup()");
		exit(-1);
	}//if	

	SOCKET lsd = socket(AF_INET, SOCK_STREAM, 0);
	if (lsd == INVALID_SOCKET)
	{
		_ERROR("socket()");
		WSACleanup();
		exit(-1);
	}//if

	//server IP:127.0.0.1  port : 9999

	struct sockaddr_in server;
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(9999);
	server.sin_addr.s_addr =inet_addr("127.0.0.1");
	if (
		bind(lsd, (struct sockaddr*)&server,
			sizeof(server)) == SOCKET_ERROR)
	{
		_ERROR("bind()");
		closesocket(lsd);
		WSACleanup();
		exit(-1);
	}

	if (listen(lsd, 5) == SOCKET_ERROR)
	{
		_ERROR("listen()");
		closesocket(lsd);
		WSACleanup();
		exit(-1);
	}

	printf("TCP服务器程序在端口号：9999上等待连接......\n");
	struct sockaddr_in caddr;
	int len = sizeof(caddr);
	SOCKET clisd = accept(lsd,
		(struct sockaddr*)&caddr, &len);
	if (clisd == INVALID_SOCKET)
	{
		_ERROR("accept()");
		closesocket(lsd);
		WSACleanup();
		exit(-1);
	}

	printf("客户机IP：%s 端口号：%d 已连接进来！\n"
		, inet_ntoa(caddr.sin_addr), ntohs(caddr.sin_port));

	char data[254];
	int cc = 0;

	do {
		int rc = recv(clisd, data, 250, 0);
		if (rc == 0) break;
		if (rc == SOCKET_ERROR)
		{
			_ERROR("recv()");
			closesocket(clisd);
			closesocket(lsd);
			WSACleanup();
			exit(-1);
		}
		data[rc] = 0;
		printf("客户机数据：%s \n", data);
		cc = count(data);

		char rdata[254];
		printf(rdata, "字母个数：%d \n", cc);
		rc = send(clisd, rdata, strlen(rdata), 0);
		if (rc == SOCKET_ERROR)
		{
			_ERROR("send()");
			closesocket(clisd);
			closesocket(lsd);
			WSACleanup();
			exit(-1);
		}
	} while (strcmp(data, "quit") != 0);
	printf("谢谢，再见！");
	closesocket(clisd);
	closesocket(lsd);
	WSACleanup();

}//main