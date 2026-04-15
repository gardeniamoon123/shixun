/*===============================================
*   文件名称：client.c
*   创 建 者：     
*   创建日期：2026年04月15日
*   描    述：
================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVER_PORT 8000    // 服务器端口
#define SERVER_IP "192.168.110.75" // 服务器IP
#define BUF_SIZE 1024

int main(int argc, char const *argv[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUF_SIZE] = {0};
    char message[BUF_SIZE] = {0};

    // 1. 创建套接字
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error\n");
        return -1;
    }

    // 2. 配置服务器地址
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);

    if(inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        printf("地址错误或者地址不支持\n");
        return -1;
    }

    // 3. 连接服务器
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("连接失败\n");
        return -1;
    }
    printf("连接到服务器 %s:%d\n", SERVER_IP, SERVER_PORT);

    printf("可以发送信息 (输入 exit 断开连接, Ctrl+C 退出):\n");
    
    // 4. 通信循环
    while(1) {
        // 清空缓冲区
        memset(message, 0, BUF_SIZE);
        memset(buffer, 0, BUF_SIZE);
        
        // 从控制台读取用户输入
        fgets(message, BUF_SIZE, stdin);
        
        // 去除输入的换行符
        message[strcspn(message, "\n")] = 0;
        
        // 输入 exit 断开连接
        if (strcmp(message, "exit") == 0) {
            printf("主动断开连接\n");
            break;
        }

        // 发送消息给服务端
        send(sock, message, strlen(message), 0);
        printf("发送成功！\n");
        
        // 接收服务端的回显
        read(sock, buffer, BUF_SIZE);
        printf("服务端回复：%s\n", buffer);
    }

    // 5. 关闭套接字
    close(sock);
    printf("成功关闭.\n");
    return 0;
}
