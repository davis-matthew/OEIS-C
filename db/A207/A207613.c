/*
 * A207613  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A207612; see Formula section.
 * https://oeis.org/A207613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A207613: flattened coefficients of the polynomials v(n,x)
   defined by
        u(1,x)=1,  v(1,x)=1
        u(n,x)=u(n-1,x)+v(n-1,x)
        v(n,x)=u(n-1,x)+2*x*v(n-1,x)+1
   The triangle of coefficients of v(n,x) is listed row‑wise and this
   function returns the n‑th element in that flattening (a(1)=1). */

#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
/* C++: use new/delete for array construction/destruction. */
static seq_t* alloc_seq_t(size_t cap) { return new seq_t[cap]; }
static void   free_seq_t(seq_t* p)   { delete[] p; }
static seq_t* realloc_seq_t(seq_t* old, size_t old_cap, size_t new_cap)
{
    seq_t* newp = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap && i < new_cap; ++i) newp[i] = old[i];
    delete[] old;
    return newp;
}
#else
/* C: use malloc/realloc/free; seq_t assumed trivially constructible. */
static seq_t* alloc_seq_t(size_t cap) { return (seq_t*)malloc(sizeof(seq_t)*cap); }
static void   free_seq_t(seq_t* p)    { free(p); }
static seq_t* realloc_seq_t(seq_t* old, size_t old_cap, size_t new_cap)
{
    (void)old_cap; /* unused in C */
    return (seq_t*)realloc(old, sizeof(seq_t)*new_cap);
}
#endif

seq_t a(idx_t n)
{
    /* static state for incremental computation */
    static seq_t *a_vals   = NULL; /* flattened sequence, 0-indexed */
    static size_t a_cap   = 0;     /* capacity of a_vals */
    static size_t a_len   = 0;     /* number of values stored (>=1 after init) */

    static seq_t *u = NULL;       /* current u coefficients */
    static seq_t *v = NULL;       /* current v coefficients */
    static size_t u_len = 0;      /* length of u array */
    static size_t v_len = 0;      /* length of v array */
    static idx_t  cur_row = 0;    /* last computed row index (>=1) */

    /* initial row (n = 1) */
    if (cur_row == 0) {
        cur_row = 1;
        u_len = 1;
        v_len = 1;
        u = alloc_seq_t(u_len);
        v = alloc_seq_t(v_len);
        u[0] = (seq_t)1;
        v[0] = (seq_t)1;

        a_cap = 16;
        a_vals = alloc_seq_t(a_cap);
        a_vals[a_len++] = v[0];   /* a(1) = 1 */
    }

    /* generate rows until we have at least n terms */
    while (a_len < (size_t)n) {
        idx_t next_row   = cur_row + 1;
        size_t new_u_len = (size_t)next_row - 1; /* = cur_row */
        size_t new_v_len = (size_t)next_row;     /* = cur_row + 1 */

        seq_t *new_u = alloc_seq_t(new_u_len);
        seq_t *new_v = alloc_seq_t(new_v_len);

        /* u(n,x) = u(n-1,x) + v(n-1,x) */
        for (size_t i = 0; i < new_u_len; ++i) {
            seq_t ui = (i < u_len ? u[i] : (seq_t)0);
            seq_t vi = (i < v_len ? v[i] : (seq_t)0);
            new_u[i] = ui + vi;
        }

        /* v(n,x) = u(n-1,x) + 2x * v(n-1,x) + 1 */
        for (size_t d = 0; d < new_v_len; ++d) {
            seq_t part_u = (d < u_len ? u[d] : (seq_t)0);
            seq_t part_v = (d == 0 ? (seq_t)0 : ( (seq_t)2 * v[d-1] ));
            seq_t part_c = (d == 0 ? (seq_t)1 : (seq_t)0);
            new_v[d] = part_u + part_v + part_c;
        }

        /* ensure capacity for the flattened sequence */
        if (a_len + new_v_len > a_cap) {
            size_t new_cap = a_cap;
            while (a_len + new_v_len > new_cap) new_cap *= 2;
            a_vals = realloc_seq_t(a_vals, a_cap, new_cap);
            a_cap = new_cap;
        }

        /* append new_v coefficients to flattened array */
        for (size_t i = 0; i < new_v_len; ++i) {
            a_vals[a_len++] = new_v[i];
        }

        /* free old u and v arrays and update state */
        free_seq_t(u);
        free_seq_t(v);
        u      = new_u;
        v      = new_v;
        u_len  = new_u_len;
        v_len  = new_v_len;
        cur_row = next_row;
    }

    return a_vals[(size_t)n - 1];
}
