/*symbol table related functions used in semantic analysis*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "table.h"
#include "tree.h"
#include "type.h"
#include "common/tokenset.h"

/* three tables will be build in semantic analysis
 * 1. func def table
 * 2. struct def table
 * 3. var def table 
 * a structure definition will be insert into struct def table
 * a structure variable definition will be insert into var def table
 */

struct func_descriptor func_head;	//empty head
struct func_descriptor* func_table_head = &func_head;

struct struct_descriptor struct_head;
struct struct_descriptor* struct_table_head = &struct_head;

struct var_descriptor var_head;		
struct var_descriptor* var_table_head = &var_head;


/******************************functions for func_table******************************/

/*create a new func symbol to represent a function definition
 *each new func symbol will be add into func table*/
struct func_descriptor*
create_func_descriptor(char *func_name, struct type_descriptor* return_type, int param_num){
	
	assert(strlen(func_name) < 20);
	struct func_descriptor *new_func_descriptor = (struct func_descriptor *)malloc(sizeof(struct func_descriptor));
	strcpy(new_func_descriptor -> func_name, func_name);
	new_func_descriptor -> return_type = return_type;
	new_func_descriptor -> param_num = param_num;
	new_func_descriptor -> next = NULL;

	return new_func_descriptor;
}

/*find func descriptor. NULL when not found*/
struct func_descriptor*
find_func(struct func_descriptor* head, char *func_name){
	assert(strlen(func_name) < 20);
	struct func_descriptor* p;
	for ( p = head -> next; p != NULL; p = p -> next){
		if(strcmp(p -> func_name, func_name) == 0)
			break;
	}
	return p;
}

/*add a new func into func table (at the head of the list)
 *note: repeat checking should have been done before this*/
void 
add_func(struct func_descriptor* head, struct func_descriptor *new_func){
		assert(head != NULL);
		assert(new_func != NULL);
		new_func -> next = head -> next;
		head -> next = new_func;
}

void
print_func_table(struct func_descriptor* head){
	int i = 0;
	struct func_descriptor* p;
	printf("+======================function table========================+\n");
	printf("%29s", "function name");
	printf("%15s", "return type");
	printf("%17s", "param numbers");
	for ( p = head -> next; p != NULL; p = p -> next){
		printf("\n %d\t ", i++);
		printf("%20s", p -> func_name);
		printf("%15s", type_ctos(p -> return_type -> type_code));
		printf("%17d", p -> param_num);
	}
	printf("\n+============================================================+\n");
}

//todo : destructors

/******************************functions for struct_table******************************/

struct struct_descriptor*
create_struct_descriptor(char* struct_name/*, struct tree_node* deflist*/){
	assert(strlen(struct_name) < 20);
	struct struct_descriptor* new_struct_descriptor = (struct struct_descriptor*)malloc(sizeof(struct struct_descriptor));
	strcpy(new_struct_descriptor -> struct_name, struct_name);
	new_struct_descriptor -> member_list_head = create_var_descriptor("", NULL, NULL);//empty head
	new_struct_descriptor -> next = NULL;
	new_struct_descriptor -> member_num = -1;
	return new_struct_descriptor;
}


struct struct_descriptor*
create_struct_descriptor_by_structspecifier(struct tree_node *structspecifier_node){
	assert(structspecifier_node -> unit_code == StructSpecifier);
	assert(structspecifier_node -> child -> sibling -> unit_code == OptTag);
	struct tree_node* opttag_node = structspecifier_node -> child -> sibling;
	char* struct_name;
	if(opttag_node -> child == NULL)//anonymous structure def
		struct_name = "";
	else
		struct_name = opttag_node -> child -> unit_value;
	//todo : check name repeat???
	//todo : member list
	return create_struct_descriptor(struct_name);

}


struct struct_descriptor*
find_struct(struct struct_descriptor* head, char* struct_name){
	assert(strlen(struct_name) < 20);
	struct struct_descriptor* p;
	for ( p = head -> next; p != NULL; p = p -> next){
		if(strcmp(p -> struct_name, struct_name) == 0)
			break;
	}
	return p;
}

void
add_struct(struct struct_descriptor* head, struct struct_descriptor* new_struct){
	assert(head != NULL);
	assert(new_struct != NULL);
	new_struct -> next = head -> next;
	head -> next = new_struct;
}

void
print_struct_table(struct struct_descriptor* head){
	int i = 0;
	struct struct_descriptor* p;
	printf("\n+==============structure table================+\n");
	printf("%29s", "structure name");
	printf("%17s", "menber numbers");
	for ( p = head -> next; p != NULL; p = p -> next){
		printf("\n %d\t ", i++);
		printf("%20s", p -> struct_name);
		printf("%17d", p -> member_num);
	}
	printf("\n+=============================================+\n");
}

/******************************functions for var_table******************************/

struct var_descriptor*
create_var_descriptor(char* var_name, struct type_descriptor *var_type, struct array_descriptor *var_array){
	assert(strlen(var_name) < 20);
	struct var_descriptor* new_var_descriptor = (struct var_descriptor* )malloc(sizeof(struct var_descriptor));
	strcpy(new_var_descriptor -> var_name, var_name);
	new_var_descriptor -> var_type = var_type;
	new_var_descriptor -> var_array = var_array;
	new_var_descriptor -> next = NULL;
	
	return new_var_descriptor;
}

struct var_descriptor*
find_var(struct var_descriptor* head, char* var_name){
	assert(strlen(var_name) < 20);
	struct var_descriptor* p;
	for ( p = head -> next; p != NULL; p = p -> next){
		if(strcmp(p -> var_name, var_name) == 0)
			break;
	}
	return p;
}

void
add_var(struct var_descriptor* head, struct var_descriptor* new_var){
	assert(head != NULL);
	assert(new_var != NULL);
	new_var -> next = head -> next;
	head -> next = new_var;
}

void
print_var_table(struct var_descriptor* head){
	int i = 0;
	struct var_descriptor* p;
	printf("\n+=================variables table=============+\n");
	printf("%29s", "variable name");
	printf("%17s", "variable type");
	for ( p = head -> next; p != NULL; p = p -> next){
		printf("\n %d\t ", i++);
		printf("%20s", p -> var_name);
		printf("%17d", p -> var_type -> type_code);
	}
	printf("\n+=============================================+\n");
}