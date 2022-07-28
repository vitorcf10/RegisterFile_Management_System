#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int MAX_REG = 3;
int frrn = 0;
int rrrn = 0;
struct identidade{
  char ra[7];
  char aniversario[11];
  char nome[20];
};
typedef struct identidade id;

void removeReg(FILE *arquivo, int chave){
  char comp[20];
  int compi, rrn = -1, k=-1;
  rewind(arquivo);
  do{
    fread(&comp, 7*sizeof(char), 1, arquivo);
    compi = atoi(comp);
    //printf("%d", compi);
    fseek(arquivo, 31, SEEK_CUR);
    rrn++;
    k++;
  }while(chave!=compi && k<frrn);
  rewind(arquivo);
  fseek(arquivo, rrn*38, 0);
  fwrite("0", sizeof(char)*7, 1, arquivo);
  rrrn--;
}

void insereReg(FILE* arquivo, id dados){
  rewind(arquivo);
  fseek(arquivo, frrn*38, 0);
  fwrite(&dados.ra, sizeof(dados.ra), 1, arquivo);
  fwrite(&dados.aniversario, sizeof(dados.aniversario), 1, arquivo);
  fwrite(&dados.nome, sizeof(dados.nome), 1, arquivo);
  frrn++;
  rrrn++;
}

void listaValidos(FILE* arquivo){
  char comp[20];
  int compi, rrn = -1;
  id validos;
  rewind(arquivo);
  for(int i = 0; i < frrn; i++){ 
    fread(&comp, 7*sizeof(char), 1, arquivo);
    compi = atoi(comp);
    fread(&validos.aniversario, 11*sizeof(char), 1, arquivo);
    fread(&validos.nome, 20*sizeof(char), 1, arquivo);
    if(compi != 0){
      printf("%s\n", comp);
      printf("%s\n", validos.aniversario);
      printf("%s\n", validos.nome);
    }
  }
}

void listaInvalidos(FILE* arquivo){
  char comp[20];
  int compi, rrn = -1;
  id validos;
  rewind(arquivo);
  for(int i = 0; i < frrn; i++){ 
    fread(&comp, 7*sizeof(char), 1, arquivo);
    compi = atoi(comp);
    fread(&validos.aniversario, 11*sizeof(char), 1, arquivo);
    fread(&validos.nome, 20*sizeof(char), 1, arquivo);
    if(compi == 0){
      printf("%s\n", comp);
      printf("%s\n", validos.aniversario);
      printf("%s\n", validos.nome);
    }
  }
}

void mudaReg(FILE *arquivo, int chave){//nao deixar mudar ra para 0
  id muda;
  char comp[20];
  int compi, rrn = -1, opcao, k = -1;
  rewind(arquivo);
  do{
    fread(&comp, 7*sizeof(char), 1, arquivo);
    compi = atoi(comp);
    //printf("%d", compi);
    fseek(arquivo, 31, SEEK_CUR);
    rrn++;
    k++;
  }while(chave!=compi && k < frrn);
  rewind(arquivo);
  fseek(arquivo, rrn*38, 0);
  printf("Qual campo voce deseja alterar? 1 = RA; 2 = Aniversario; 3 = Nome:");
  scanf("%d", &opcao);
  if(opcao == 1){
    printf("Escreva o novo RA:");
    scanf("%s", muda.ra);
    fwrite(&muda.ra, sizeof(char)*7, 1, arquivo);
  }else if (opcao == 2){
    printf("Escreva o novo aniversario:");
    scanf("%s", muda.aniversario);
    fseek(arquivo, 7, SEEK_CUR);
    fwrite(&muda.aniversario, sizeof(char)*11, 1, arquivo);
  }else if(opcao == 3){
    printf("Escreva o novo nome: ");
    scanf("%s", muda.nome);
    fseek(arquivo, 18, SEEK_CUR);
    fwrite(&muda.nome, sizeof(char)*20, 1, arquivo);
  }else{
    printf("Opcao invalida, registro nao alterado!");
    return;
  }
  rewind(arquivo); 
}

void buscaPorChavePrimaria(FILE* arquivo, int chave){
  char comp[20];
  id acha;
  int compi, rrn = -1, k=-1;
  rewind(arquivo);
  do{
    fread(&comp, 7*sizeof(char), 1, arquivo);
    compi = atoi(comp);
    //printf("%d", compi);
    fseek(arquivo, 31, SEEK_CUR);
    rrn++;
    k++;
  }while(chave!=compi && k<frrn);
  //printf(" %d ", compi);
  rewind(arquivo);
  fseek(arquivo, 38*rrn, SEEK_SET);
  fread(&acha.ra, 7*sizeof(char), 1, arquivo);
  fread(&acha.aniversario, 11*sizeof(char), 1, arquivo);
  fread(&acha.nome, 20*sizeof(char), 1, arquivo);
  if(chave == compi){
    printf("%s\n", acha.ra);
    printf("%s\n", acha.aniversario);
    printf("%s\n", acha.nome);
  }else{
    printf("O RA informado nao consta no arquivo.");
  }
}

void buscaRegAniversario(FILE* arquivo, char nasce[10]){
  char comp[20];
  int compi, rrn = -1, k=0;
  id acha;
  rewind(arquivo);
  fseek(arquivo, 7, SEEK_SET);
  do{
    fread(&comp, 11*sizeof(char), 1, arquivo);
    //printf("%s", comp);
    fseek(arquivo, 27, SEEK_CUR);
    rrn++;
    k++;
    if(!strcmp(comp, nasce)){
      //printf(" %d ", rrn);
      rewind(arquivo);
      fseek(arquivo, rrn*38, SEEK_SET);
      fread(&acha.ra, 7*sizeof(char), 1, arquivo);
      fread(&acha.aniversario, 11*sizeof(char), 1, arquivo);
      fread(&acha.nome, 20*sizeof(char), 1, arquivo);
      printf("%s\n", acha.ra);
      printf("%s\n", acha.aniversario);
      printf("%s\n", acha.nome);
      fseek(arquivo, 7, SEEK_CUR);
    }
  }while(k<frrn);
  rewind(arquivo);
}

void buscaRegNome(FILE* arquivo, char name[20]){
  char comp[30];
  int compi, rrn = -1, k=0;
  id acha;
  rewind(arquivo);
  fseek(arquivo, 18, SEEK_SET);
  do{
    fread(&comp, 20*sizeof(char), 1, arquivo);
    //printf("%s\n", comp);
    fseek(arquivo, 18, SEEK_CUR);
    rrn++;
    k++;
    if(!strcmp(comp, name)){
      //printf(" %d ", rrn);
      rewind(arquivo);
      fseek(arquivo, rrn*38, SEEK_SET);
      fread(&acha.ra, 7*sizeof(char), 1, arquivo);
      fread(&acha.aniversario, 11*sizeof(char), 1, arquivo);
      fread(&acha.nome, 20*sizeof(char), 1, arquivo);
      printf("%s\n", acha.ra);
      printf("%s\n", acha.aniversario);
      printf("%s\n", acha.nome);
      fseek(arquivo, 18, SEEK_CUR);
    }
  }while(k<frrn);
  rewind(arquivo);
}

FILE* compacta(FILE* velho){
  char comp[20];
  int compi, rrn = -1;
  id novos;
  FILE*arquivoNovo = fopen("arquivoCompactado", "w+");
  rewind(velho);
  for(int i = 0; i < frrn; i++){ 
    fread(&comp, 7*sizeof(char), 1, velho);
    compi = atoi(comp);
    fread(&novos.aniversario, 11*sizeof(char), 1, velho);
    fread(&novos.nome, 20*sizeof(char), 1, velho);
    if(compi != 0){
      fwrite(&comp, 7*sizeof(char), 1, arquivoNovo);
      fwrite(&novos.aniversario, 11*sizeof(char), 1, arquivoNovo);
      fwrite(&novos.nome, 20*sizeof(char), 1, arquivoNovo);
    }
  }
  frrn = rrrn;
  remove("arquivo");
  return arquivoNovo;
}


int main(void) {
  id p1 = {"765432", "25/07/1992", "Mario Matos"};
  id p2 = {"908712", "01/02/2011", "Roberta Oueno"};
  id p3 = {"556631", "09/12/1955", "Vera Guimaraes"};
  id p4 = {"100873", "17/09/1999", "Joao Vitor Tamiao"};
  id p5 = {"999432", "13/04/2012", "Enzo Ebert"};
  id pessoa;
  int tam;
  FILE *arquivo = fopen("arquivo", "w+"), *novo;
  insereReg(arquivo, p1);
  insereReg(arquivo, p2);
  insereReg(arquivo, p3);
  printf("Lista de validos apos 3 insercoes:\n");
  listaValidos(arquivo);
  printf("Lista de invalidos apos 3 insercoes:\n");
  listaInvalidos(arquivo);
  printf("\n");
  removeReg(arquivo, 556631);
  insereReg(arquivo, p4);
  printf("Lista de validos apos uma insercao e remocao:\n");
  listaValidos(arquivo);
  printf("Lista de invalidos apos uma insercao e remocao:\n");
  listaInvalidos(arquivo);
  printf("\n");
  printf("Altera campo de registro:\n");
  mudaReg(arquivo, 908712);
  printf("\n");
  printf("Busca por chave primaria:\n");
  buscaPorChavePrimaria(arquivo, 908712);
  printf("\n");
  printf("Busca por campo especifico:\n");
  insereReg(arquivo, p5);
  buscaRegNome(arquivo, "Enzo Ebert");
  novo = compacta(arquivo);
  printf("\n");
  printf("Lista de validos do arquivo compactado:\n");
  listaValidos(novo);
  printf("Lista de invalidos do arquivo compactado:\n");
  listaInvalidos(novo);
  fclose(arquivo);
}
