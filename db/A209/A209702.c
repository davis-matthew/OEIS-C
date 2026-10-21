/*
 * A209702  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209701; see the Formula section.
 * https://oeis.org/A209702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for OEIS A209702:
   u(1,x)=1, v(1,x)=1
   u(n,x)=x*u(n-1,x)+x*v(n-1,x)
   v(n,x)=2*x*u(n-1,x)+(x+1)*v(n-1,x)+1
   The sequence a(n) is obtained by flattening the coefficients of
   v(n,x) in increasing order of the exponent of x. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static state: caches of terms and of the last polynomial
       coefficients.  Initialization occurs on the first call. */
    static seq_t *terms = NULL;      /* flattened coefficients of v(n,x) */
    static size_t terms_cap = 0;     /* capacity of the terms array     */
    static size_t terms_len = 0;     /* number of terms stored          */

    static seq_t *u_coeff = NULL;    /* coefficients of current u(x)   */
    static seq_t *v_coeff = NULL;    /* coefficients of current v(x)   */
    static size_t coeff_cap = 0;     /* capacity of coefficient arrays */
    static size_t current_row = 0;   /* number of rows computed so far */

    if (!terms) {
        /* Initialisation for row 1 */
        current_row = 1;
        coeff_cap = 2;                /* allocate a little buffer */
        u_coeff = (seq_t*)malloc(coeff_cap * sizeof(seq_t));
        v_coeff = (seq_t*)malloc(coeff_cap * sizeof(seq_t));
        u_coeff[0] = (seq_t)1;
        v_coeff[0] = (seq_t)1;

        terms_cap = 1;
        terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
        terms[0] = (seq_t)1;
        terms_len = 1;
    }

    if (n == (idx_t)0)
        return (seq_t)0;   /* undefined for n=0 */

    /* Generate terms up to the requested index if necessary */
    while ((size_t)n > terms_len) {
        size_t new_row = current_row + 1;          /* next row size */

        /* Allocate space for the new coefficients */
        seq_t *new_u = (seq_t*)malloc(new_row * sizeof(seq_t));
        seq_t *new_v = (seq_t*)malloc(new_row * sizeof(seq_t));

        /* Compute new_u:  u_n(x) = x*u_{n-1}(x) + x*v_{n-1}(x) */
        new_u[0] = (seq_t)0;
        for (size_t i = 1; i < new_row; ++i) {
            new_u[i] = u_coeff[i-1] + v_coeff[i-1];
        }

        /* Compute new_v:  v_n(x) = 2x*u_{n-1}(x) + (x+1)*v_{n-1}(x) + 1 */
        for (size_t i = 0; i < new_row; ++i) {
            seq_t val = (seq_t)0;
            if (i > 0)           /* 2x*u_{n-1} contribution    */
                val += (seq_t)2 * u_coeff[i-1];
            if (i < current_row) /* (x+1)*v_{n-1} part (old_v[i]) */
                val += v_coeff[i];
            if (i > 0)           /* (x+1)*v_{n-1} part (old_v[i-1]) */
                val += v_coeff[i-1];
            if (i == 0)          /* constant +1 */
                val += (seq_t)1;
            new_v[i] = val;
        }

        /* Extend the terms array to store the new row */
        if (terms_len + new_row > terms_cap) {
            size_t new_cap = terms_cap ? terms_cap * 2 : 1;
            while (new_cap < terms_len + new_row) new_cap <<= 1;
            terms = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
            terms_cap = new_cap;
        }
        for (size_t i = 0; i < new_row; ++i) {
            terms[terms_len++] = new_v[i];
        }

        /* Release old coefficient arrays and set new ones as current */
        free(u_coeff);
        free(v_coeff);
        u_coeff = new_u;
        v_coeff = new_v;
        current_row = new_row;
    }

    return terms[(size_t)n - 1];
}
