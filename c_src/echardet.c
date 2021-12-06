#include <erl_nif.h>
#include <uchardet.h>
#include <string.h>

#define MAXBUFLEN 1024

static ERL_NIF_TERM detect_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    ErlNifBinary input;
    if(!enif_inspect_iolist_as_binary(env, argv[0], &input)) {
        return enif_make_badarg(env);
    }

    uchardet_t handle = uchardet_new();
    int retval = uchardet_handle_data(handle, input.data, input.size);
    if (retval != 0){
        return enif_make_badarg(env);
    }
    uchardet_data_end(handle);
    char * charset = strdup(uchardet_get_charset(handle));
    uchardet_delete(handle);

    if (*charset)
        return enif_make_string(env, charset, ERL_NIF_LATIN1);
    else
        return enif_make_string(env, "ascii/unknown", ERL_NIF_LATIN1);
}

static ErlNifFunc nif_funcs[] = {
    {"detect", 1, detect_nif}
};

ERL_NIF_INIT(echardet, nif_funcs, NULL, NULL, NULL, NULL)
