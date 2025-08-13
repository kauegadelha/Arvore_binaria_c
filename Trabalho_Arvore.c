#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>
#include <string.h>

typedef struct elementos {
    char rgm[9];
    char nome[50];
} t_elemento;

typedef struct no {
    struct no * esq;
    t_elemento aluno;
    struct no * dir;
} t_no;

typedef t_no* t_arvore;

t_no * criar () {
    t_no * no = (t_no*) malloc(sizeof(t_no));

    if (no)
        no->esq = no->dir = NULL;

    return no;
}

t_arvore  inicializarArvore() {
	return NULL;
}

int arvoreIsVazia(t_arvore tree) {
    return (tree == NULL);
}

int isValidRgm(char rgm[]) {
	int i;

	if (strlen(rgm) != 8) {
		return 0;
	}
	for (i = 0; i < 8; i++) {
		if (!isdigit(rgm[i])) {
			return 0;
		}
	}
	return 1;
}

int compara (char aluno[], char dado[]) {
	return (strcmp(aluno, dado));
}

t_no * busca(t_arvore tree, t_elemento dado) {
	int comp;

    if (tree == NULL)
        return NULL;

    comp = compara(tree->aluno.rgm, dado.rgm);

    if (comp == 0) {
    	return tree;

	} else if (comp > 0) {
        return busca(tree->esq, dado);

	} else
        return busca(tree->dir, dado);
}

int inserir (t_arvore *tree, t_elemento item) {
    int ok;

    if (*tree == NULL) {
        *tree = criar();
        if (*tree == NULL)
            return 0;
        (*tree)->aluno = item;
        return 1;
    }
    if (compara((*tree)->aluno.rgm, item.rgm)<0)
        ok = inserir (&((*tree)->dir), item);
    else
        if (compara((*tree)->aluno.rgm, item.rgm)>0)
            ok = inserir (&((*tree)->esq), item);
        else
            ok = 0;
    return ok;
}

int inserirArquivo(t_arvore * tree) {
	FILE *file;
    char path[] = "Alunos.txt";
    t_elemento dado;

    file = fopen(path, "r");

    if (file == NULL) {
		perror("Erro ao abrir arquivo");
		return 0;
	}

    while (fscanf(file, "%s %49[^\n]\n", dado.rgm, dado.nome) == 2) {
        inserir(&(*tree), dado);
    }
    fclose(file);

    return 1;
}


void exibirPreOrdem (t_arvore tree) {
    if (tree!=NULL) {
        printf("\n| ALUNO | RGM: %s\t| NOME: %s |", tree->aluno.rgm, tree->aluno.nome);
        exibirPreOrdem(tree->esq);
        exibirPreOrdem(tree->dir);
    }
}

void exibirInOrdem (t_arvore tree) {
    if (tree!=NULL) {
        exibirInOrdem(tree->esq);
        printf("\n| ALUNO | RGM: %s | NOME: %s |", tree->aluno.rgm, tree->aluno.nome);
        exibirInOrdem(tree->dir);
    }
}

void exibirPosOrdem (t_arvore tree) {
    if (tree!=NULL) {
        exibirPosOrdem(tree->esq);
        exibirPosOrdem(tree->dir);
        printf("\n| ALUNO | RGM: %s | NOME: %s |", tree->aluno.rgm, tree->aluno.nome);
    }
}

t_no * buscaSetPai (t_arvore tree, t_elemento dado, t_no ** pai) {

    if (tree == NULL) {
        *pai = NULL;
        return NULL;
    }
    if (compara(tree->aluno.rgm, dado.rgm)==0)
        return tree; // retorna o filho, o n� a ser removido

    if (compara(tree->aluno.rgm, dado.rgm)>0) {
        *pai = tree;// modifica o ponteiro pai
        return buscaSetPai(tree->esq, dado, pai); // procura pelo filho esquerdo de pai
    }
    else {
        *pai = tree;
        return buscaSetPai(tree->dir, dado, pai); // procura pelo filho direito de pai
    }
}

int remover (t_arvore *tree, t_elemento item) {
    // Declara��o de ponteiros auxiliares
    t_no *no, *pai, *sub, *paiSuce, *suce;

    // Inicializa 'pai' como NULL
    pai = NULL;

    // Busca o n� que ser� removido e tamb�m seu pai
    no = buscaSetPai(*tree, item, &pai);

    // Se n�o encontrou o n�, retorna 0 (n�o existe)
    if (no == NULL)
        return 0;

	// Caso 1: n� folha (sem filhos) ou
	// Caso 2: n� com apenas filho � direita
    if (no->esq == NULL) {
        sub = no->dir;
    }
    else {
        // Caso 3: n� n�o possui filho � direita
        if (no->dir == NULL) {
            sub = no->esq;
        }
        else {
            // Caso 4: n� possui dois filhos
            paiSuce = no;             // pai do sucessor come�a como o pr�prio n�
            sub = no->dir;            // come�a pela sub�rvore direita
            suce = sub->esq;          // tenta ir para a esquerda (para encontrar o menor valor)

            // Procura o sucessor (menor n� da sub�rvore direita)
            while (suce != NULL) {
                paiSuce = sub;
                sub = suce;
                suce = sub->esq;
            }

            // Se o sucessor n�o for filho direto do n� que ser� removido
            if (paiSuce != no) {
                paiSuce->esq = sub->dir; // Atualiza o filho esquerdo do pai do sucessor
                sub->dir = no->dir;      // O sucessor recebe o filho direito do n� removido
            }

            // O sucessor assume o filho esquerdo do n� removido
            sub->esq = no->esq;
        }
    }

    // Se o n� a ser removido era a raiz da �rvore
    if (pai == NULL) {
        *tree = sub;
    }
    else {
        // Se o n� a ser removido era filho � esquerda do pai
        if (no == pai->esq) {
            pai->esq = sub;
        }
        // Se era filho � direita do pai
        else {
            pai->dir = sub;
        }
    }

    // Mostra na tela quem foi removido
    printf("\n| ALUNO | RGM: %s | NOME: %s |\n", no->aluno.rgm, no->aluno.nome);

    // Libera a mem�ria do n� removido
    free(no);

    // Retorna 1 (sucesso)
    return 1;
}


void esvaziar (t_arvore *tree) {
    if (*tree == NULL)
        return;
    esvaziar(&(*tree)->esq);
    esvaziar(&(*tree)->dir);
    free(*tree);
    *tree = NULL;
}

void menuEquipe () {
	printf("                 EQUIPE                       ");
	printf("\n----------------------------------------------");
	printf("\nALUNO: KAU� TORRES PEREIRA DE ABRANTES GADELHA.");
	printf("\nALUNO: CAIO DIAS COSTA.");
	printf("\nALUNO: JOSE CARLOS MARINHO DE LIMA.");
	printf("\nALUNO: MAILSON GOMES DE ANDRADE.");
	printf("\n----------------------------------------------");
	printf("\nDISCIPLINA: ESTRUTURA DE DADOS 1.");
	printf("\nPROFESSOR: WALACE BONFIM.");
	printf("\n----------------------------------------------");
}

void menu () {
	printf("\n\n\t=============================================================\n");
    printf("\t|               EDITOR DE �RVORE                            |\n");
	printf("\t| OP [1] - INSERIR (Fornecer RGM e nome do aluno).          |\n");
	printf("\t| OP [2] - REMOVER UM N� (fornecer o RGM a remover).        |\n");
	printf("\t| OP [3] - PESQUISAR (Fornecer RGM a pesquisar).            |\n");
    printf("\t| OP [4] - ESVAZIAR A �RVORE.                               |\n");
    printf("\t| OP [5] - EXIBIR A �RVORE (Tr�s op��es: PR�, IN ou P�S).   |\n");
    printf("\t| OP [0] - SAIR.                                            |\n");
    printf("\t=============================================================\n");
}

int main () {
	setlocale(LC_ALL, "Portuguese");
	t_arvore tree;
	tree = inicializarArvore();
	t_no * aux;
	t_elemento dado;
	int op, opExibicao;

	if (inserirArquivo(&tree) == 0) {
		exit(1);
	}
	menuEquipe();
	do {
		menu();
		printf("\nDigite uma op��o: ");
		if (scanf("%d", &op) != 1) {
			while(getchar() != '\n');
			op = -1;
		}

		switch(op) {
			case 1: printf("\nDigite o rgm do aluno: ");
					scanf("%s", dado.rgm);
					getchar();

					if (isValidRgm(dado.rgm) == 0) {
						printf("\nRgm inv�lido!");
						break;
					}

					printf("\nDigite o nome do aluno: ");
					scanf("%[^\n]", dado.nome);
					getchar();

					if (inserir(&tree, dado) == 1) {
						printf("\nInserido com sucesso.");
					} else
						printf("\nN�o foi possivel inserir aluno.");
					break;

			case 2: if (arvoreIsVazia(tree)) {
    					printf("\n�rvore est� vazia.");
					} else {
						printf("\nDigite o rgm do aluno para remover: ");
						scanf("%s", dado.rgm);
						getchar();

						if (remover(&tree, dado) == 1) {
							printf("\nRemovido com sucesso.");
						} else
							printf("\nN�o foi possivel remover.");
					}
					break;

			case 3: if (arvoreIsVazia(tree)) {
    					printf("\n�rvore est� vazia.");
					} else {
						printf("\nDigite o rgm para pesquisar: ");
						scanf("%s", dado.rgm);

						aux = busca(tree, dado);
						if (aux != NULL){
							printf("\nAluno encontrado:\n");
							printf("\n| ALUNO | RGM: %s | NOME: %s |\n",aux->aluno.rgm, aux->aluno.nome);
						} else
							printf("\nAluno n�o encontrado.");
					}
					break;

			case 4: esvaziar(&tree);
					printf("\n�rvore esvaziada com sucesso.");
					break;

            case 5: if (arvoreIsVazia(tree)) {
                        printf("\n�rvore est� vazia.");
                    } else {
                        printf("\nEscolha a ordem de exibi��o:");
                        printf("\n1 - Pr�-Ordem");
                        printf("\n2 - In-Ordem");
                        printf("\n3 - P�s-Ordem");
                        printf("\nOp��o: ");
                        scanf("%d", &opExibicao);

                        if (opExibicao == 1) {
                            printf("\n\nPr�-Ordem:\n"); exibirPreOrdem(tree);
                        } else if (opExibicao == 2) {
                            printf("\n\nIn-Ordem:\n");  exibirInOrdem(tree);
                        } else if (opExibicao == 3) {
                            printf("\n\nP�s-Ordem:\n"); exibirPosOrdem(tree);
                        } else {
                            printf("\nOp��o inv�lida!");
                        }
                    }
                    break;

			case 0: esvaziar(&tree);
					printf("\nEncerrando...");
					break;

			default: printf("\nOp��o incorreta!\n");
		}

	} while(op != 0);

	return 0;
}
