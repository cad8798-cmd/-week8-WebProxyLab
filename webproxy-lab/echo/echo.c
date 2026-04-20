#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>


#define PORT 8080
#define BUF_SIZE 1024



int main(){
    int server_socket, clinet_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_size;
    char buffer[BUF_SIZE];
    int read_len;

    server_socket = socket(PF_INET, SOCK_STREAM, 0);

    if(server_socket == -1) {
        perror("socket()error");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;

    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    server_addr.sin_port = htons(PORT);

    if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){
        perror("bind() error");
        exit(1);
    }

    if(listen(server_socket, 5) == -1){
        perror("listen() error");
        exit(1);
    }

    printf("에코 서버가 %d 포트에서 대기 중입니다... \n", PORT);

    while(1){
    client_addr_size = sizeof(client_addr);
    
    // 클라이언트 연결 수락
    clinet_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_size);
    
    // 데이터 수신 후 그대로 돌려보내기 (에코)
    while((read_len = read(clinet_socket, buffer, BUF_SIZE)) > 0){

        printf("%d\n", read_len);
        if ((read_len == 4 && memcmp(buffer, "ping", 4) == 0) ||
            (read_len == 5 && memcmp(buffer, "ping\n", 5) == 0) ||
            (read_len == 6 && memcmp(buffer, "ping\r\n", 6) == 0)) {
            write(clinet_socket, "pong", 4);
        } else {
            write(clinet_socket, buffer, read_len);
        }
    }
    
    close(clinet_socket);
}

}
