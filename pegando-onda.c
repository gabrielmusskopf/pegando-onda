#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <conio.h>
#include <windows.h>
#include <unistd.h>

#define NOME 50
#define DIR 50

struct dados{                                                                             //Struct de dados
    char Chunk_ID[5];
    int Chunk_Size;
    char Format[5];
    char SubChunk_1ID[5];
    int SubChunk_1Size;
    short int Audio_Format;                                                               //Short pq são só 2 bytes
    short int Num_Channels;
    int Sample_Rate;
    int Byte_Rate;
    short int Block_Align;
    short int Bits_Per_Sample;
    char SubChunk_2ID[5];
    int SubChunk_2Size;
    int Data;
};

int main(void)
{
    setlocale(LC_ALL, "Portuguese");
    system("color");
    struct dados info;
    int i,escolha=0;
    char nome_arquivo[NOME];
    char teste_riff[5];
    int teste_size;
    char teste_wave[5];
    char diretorio[DIR]="C:\\Users\\Tatiana\\Desktop\\";
    int corte_origem,corte_final;
    int inicio,fim;


    printf("Digite o nome do arquivo:");
    gets(nome_arquivo);
    strcat(diretorio,nome_arquivo);

    //printf("\n%s \n\n",diretorio);

    FILE *input;
    input=fopen(diretorio,"r");

        if(input == NULL)                                                                    //Teste de arquivo
        {
            printf("\033[0;31m");printf("\n\t   FILE NOT FOUND!\n");printf("\033[0;0m");
            printf("O arquivo '%s' não existe nesse diretório!\n",nome_arquivo);
            return 0;
        }

    fread(&teste_riff, sizeof(char), 4, input);
    teste_riff[4] = '\0';
    fread(&teste_size, sizeof(int), 1, input);
    fread(&teste_wave, sizeof(char), 4, input);
    teste_wave[4]='\0';

    if(teste_riff[0]!='R')                                                                //Teste de formato
    {
        if(teste_wave[0]!= 'W')
        {
            printf("\033[0;31m");printf("\n\t   ERROR\n");printf("\033[0;0m");
            printf("O arquivo '%s' não é .wav\n",nome_arquivo);
            return 0;
        }
    }


    printf("\033[0;32m");
    printf("\tBem vindo!\n      O que desejas?\n");
    printf("\033[0;0m");
    do{
        rewind(input);                                                                       //Reinicia o ponteiro

        printf("\033[0;33m");
        printf(" 1 - Abrir o arquivo\n");
        printf(" 2 - Exibir as informações do arquivo\n");
        printf(" 3 - Filtrar áudio\n");
        printf(" 4 - Recortar\n");
        printf(" 5 - Reiniciar\n");
        printf("\033[0;0m");
        scanf("%d",&escolha);

    switch(escolha)
    {

        case(2):{
        printf("\n      Informações:\n");
        //Leitura do Chunk Descriptor
        fread(&info.Chunk_ID, sizeof(char), 4, input);                                       //Le Chunk ID
        info.Chunk_ID[4] = '\0';
        printf("\nCHUNK ID:\t %s \n",info.Chunk_ID);

        fread(&info.Chunk_Size, sizeof(int), 1, input);                                      //Lê Chunk Size
        printf("CHUNK SIZE:\t %d bytes \n",info.Chunk_Size);                              //32bits = 1 // 16 bits = 2

        fread(&info.Format, sizeof(char), 4, input);                                         //Lê Format
        info.Format[4]='\0';
        printf("FORMAT:\t\t %s \n\n",info.Format);

        //Leitura do Sub-Chunk fmt
        fread(&info.SubChunk_1ID, sizeof(char), 4, input);
        info.SubChunk_1ID[4]='\0';
        printf("SUBCHUNK 1 ID:\t %s \n",info.SubChunk_1ID);

        fread(&info.SubChunk_1Size, sizeof(int), 1, input);
        printf("SUBCHUNK 1 SIZE: %d bytes \n",info.SubChunk_1Size);

        fread(&info.Audio_Format, sizeof(short int), 1, input);
        printf("AUDIO FORMAT:\t %d (PCM) \n",info.Audio_Format);

        fread(&info.Num_Channels, sizeof(short int), 1, input);
        if(info.Num_Channels==1)
            {
                printf("NUM CHANNELS:\t %d (MONO)\n",info.Num_Channels);
            }else printf("NUM CHANNELS:\t %d \n",info.Num_Channels);

        fread(&info.Sample_Rate, sizeof(int), 1, input);
        printf("SAMPLE RATE:\t %d \n",info.Sample_Rate);

        fread(&info.Byte_Rate, sizeof(int), 1, input);
        printf("BYTE RATE:\t %d \n",info.Byte_Rate);

        fread(&info.Block_Align, sizeof(short int), 1, input);
        printf("BLOCK ALIGN:\t %d \n",info.Block_Align);

        fread(&info.Bits_Per_Sample, sizeof(short int), 1, input);
        printf("BITS PER SAMPLE: %d \n\n",info.Bits_Per_Sample);

        //Leitura Subchunk data
        fread(&info.SubChunk_2ID, sizeof(char), 4, input);
        info.SubChunk_2ID[4]='\0';
        printf("SUBCHUNK 2 ID:\t %s \n",info.SubChunk_2ID);

        fread(&info.SubChunk_2Size, sizeof(int), 1, input);
        printf("SUBCHUNK 2 SIZE: %d \n\n",info.SubChunk_2Size);

        }break;

        case 4:{

            printf("Digite o tempo que deseja começar (em segundos): ");
            scanf("%d",&corte_origem);

            inicio = corte_origem * info.Byte_Rate;  //qual byte inicia a musica;

            printf("Digite o tempo que deseja terminar (em segundos): ");
            scanf("%d",&corte_final);

            if(corte_final < corte_origem)
            {
                printf("\nTempo final maior que inicial.\nDigite novamente.\n\n");
                printf("Digite o tempo que deseja começar (em segundos): ");
                scanf("%d",&corte_origem);
                printf("Digite o tempo que deseja terminar (em segundos): ");
                scanf("%d",&corte_final);
            }

            fim = corte_final * info.Byte_Rate;     //qual byte termina a música;

            printf("Início da música em %d bytes\n",inicio);
            printf("Fim da música em %d bytes\n",fim);
        }break;

        case (5): system("cls");
            printf("\033[0;32m");
            printf("\tBem vindo!\n      O que desejas?\n");
            printf("\033[0;0m");
            break;
    }
    rewind(input);
    }while(escolha>0 && escolha<6);

    printf("\nPrograma encerrado!\n");


    fclose(input);

}
