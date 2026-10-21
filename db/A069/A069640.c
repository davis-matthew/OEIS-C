/*
 * A069640  Let M_n be the n X n matrix with M_n(i,j)=1/(i+j+1); then
 *          a(n)=1/det(M_n).
 * https://oeis.org/A069640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n+1)!! * ( n! * Π_{k=0}^{n} Catalan(k) )^2
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required includes for both C and C++ */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* The type seq_t is supplied by the build system; it may be an
 * unsigned integer of varying width or an arbitrary-precision
 * integer class (for which C++ compilation will be used).  */
 /* seq_t and idx_t are assumed to be defined elsewhere. */

/*---------------------------------------------------------------------*/
/*  Static state shared by all calls to a(idx_t n)                    */
/*---------------------------------------------------------------------*/
#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> a_tab;         /* 0‑based, entry 0 unused    */
#else
static seq_t *a_tab = NULL;             /* 0‑based, entry 0 unused    */
static size_t a_tab_cap = 0;
#endif

static size_t a_max = 0;                 /* biggest n already computed */
static seq_t cur_D = 1;                  /* (2*n+1)!! for current n     */
static seq_t cur_B = 1;                  /* n! * Π_{k=0}^{n} Catalan(k)*/
static seq_t cur_C = 1;                  /* Catalan(n)                  */

/*---------------------------------------------------------------------*/
/*  Compute a(n)                                                      */
/*---------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)            /* a(0) is undefined for the sequence, return 0 */
        return (seq_t)0;

#ifdef __cplusplus
    /* Ensure the vector is large enough */
    if (a_tab.size() < (size_t)n + 1)
        a_tab.resize((size_t)n + 1);
#else
    /* Allocate or grow the C array */
    if (a_tab == NULL) {
        size_t cap = (size_t)n + 1;
        if (cap < 2) cap = 2;          /* at least room for index 1 */
        a_tab = (seq_t*)malloc(cap * sizeof(seq_t));
        a_tab_cap = cap;
    } else if ((size_t)n + 1 > a_tab_cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 2) new_cap = 2;
        a_tab = (seq_t*)realloc(a_tab, new_cap * sizeof(seq_t));
        a_tab_cap = new_cap;
    }
#endif

    /* Compute any missing terms up to n */
    while (a_max < n) {
        idx_t next = a_max + 1;          /* next index to compute */

        if (next == 1) {               /* initialise from the definition */
            /*  (2*1+1)!! = 3,  B_1 = 1,  Catalan(1) = 1  */
            a_tab[1] = (seq_t)3;          /* a(1) = 3 */
            cur_D = (seq_t)3;
            cur_B = (seq_t)1;
            cur_C = (seq_t)1;
            a_max = 1;
            continue;
        }

        /* Compute Catalan(next) from Catalan(next-1) */
        /*   C_next = C_prev * (4*(next-1)+2) / (next+1) */
        seq_t num = cur_C * ((seq_t)(4 * (next - 1) + 2));
        seq_t next_C = num / ((seq_t)(next + 1));

        /* Compute B_next = B_prev * next * C_next */
        seq_t next_B = cur_B * ((seq_t)next);
        next_B = next_B * next_C;

        /* Compute D_next = D_prev * (2*next + 1) */
        seq_t next_D = cur_D * ((seq_t)(2 * next + 1));

        /* a_next = D_next * next_B^2 */
        seq_t a_next = next_D;
        a_next = a_next * next_B;
        a_next = a_next * next_B;

        a_tab[next] = a_next;

        /* Update state for the next iteration */
        cur_D = next_D;
        cur_B = next_B;
        cur_C = next_C;
        a_max = next;
    }

    return a_tab[n];
}
