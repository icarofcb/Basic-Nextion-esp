/*
        Esse é um programa apenas para o mais basico da nextion,
    envio de strings para blocos de texto, que costuma ser 80% 
    da comunicação com um display nextion na maioria dos casos.
    ID.txt="string"; 0xFF,0xFF,0xFF

        Mesmo as comunicações de envio de valores inteiros para
    o display, seja modificar alguma variável interna de cor ou
    de controle, o envio é feito por uma string que segue a regra
    ID.Variable=Value; 0xFF,0xFF,0xFF

        Após o envio de qualquer informação é necessário enviar
    sempre um array[3] = {0xFF, 0xFF, 0xFF}; que o display entende
    como comando de fim de envio.


*/

#include <driver/uart.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_now.h"
#include "esp_sleep.h"
#include <inttypes.h>
#include <stdbool.h>
#include "esp_system.h"
#include "sdkconfig.h"

const char Cmd_End[3] = {0xFF, 0xFF, 0xFF};

void Nextion_SendString(char *ID,char *string);
void Nextion_SendInt(char *ID,int var);
void sendData(const char* data);

//=============// NEXTION  //=============//
void Nextion_SendString(char *ID,char *string)
{
	char buf[50];
	sprintf(buf,"%s.txt=\"%s\"", ID, string);

    //sprintf(buf,"%s.Nextion_display_variable=\"%s\"", ID, string); // aqui um exemplo para caso seja necessário mudar alguma variavel especifica do projeto.

    sendData((char *)buf); //envio de ID.txt="string"
    sendData(Cmd_End);     //envio de 0xFF,0xFF,0xFF
}

/*
    Essa função torna um valor inteiro qualquer em uma string 
*/
void Nextion_SendInt(char *ID,int var)
{
	char buf[50];
	sprintf(buf,"%d",var);
	Nextion_SendString(ID,buf);
}

void initUART(void) {
    const uart_config_t uart_config = {
        .baud_rate = 115200,                   //BaudRate desse programa é de 115200, o display por padrao comunica a 9600,
                                               //então é preciso mudar isso na programação do display antes de executar o programa, seguindo recomendações do datasheet.
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    // We won't use a buffer for sending data.
    uart_driver_install(UART_NUM_1, 1024 * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, 1, 3, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

void sendData(const char* data)
{
    uart_write_bytes(UART_NUM_1, data, strlen(data));
}

static void displayUpdate(void* arg)
{
    for(;;) 
    {

        Nextion_SendString("t0","Hello");
        Nextion_SendString("t1","World");

        vTaskDelay(1000/portTICK_RATE_MS);
    
    }
}

int rawParaPorcentagem(int value)
{
    value = value * 100/3333;
    return value;
}

void app_main(void)
{
    initUART();

    //=============// Display config //=============//
    xTaskCreatePinnedToCore(displayUpdate, "displayUpdate", 2048, NULL, 10, NULL, tskNO_AFFINITY);

    



}