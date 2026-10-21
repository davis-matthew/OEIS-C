/*
 * A209755  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209756; see the Formula section.
 * https://oeis.org/A209755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the triangle of coefficients:
   u(1,x)=1, v(1,x)=1;
   u(n,x)=x*u(n‑1,x)+(x+1)*v(n‑1,x);
   v(n,x)=x*u(n‑1,x)+v(n‑1,x)+1.
   The flattened sequence a(n) lists the coefficients of u(n,x)
   row by row (degree 0 to n‑1). */

/* Include required headers */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static storage that is shared between calls */
    static seq_t *terms = NULL;    /* flattened sequence values */
    static seq_t *u = NULL;        /* coefficients of u for current row */
    static seq_t *v = NULL;        /* coefficients of v for current row */
    static idx_t prev_len = 0;     /* length of current row (number of coefficients) */

    /* n is a 1‑based index of the flattened sequence */
    if (n == 0) {
        return 0;  /* undefined, but guard against underflow */
    }

    /* Initialisation on first use */
    if (terms == NULL)
    {
        terms   = (seq_t *)malloc(sizeof(seq_t));    /* allocate space for first term */
        u       = (seq_t *)malloc(sizeof(seq_t));
        v       = (seq_t *)malloc(sizeof(seq_t));
        prev_len = 1;                                  /* first row length */
        u[0] = 1;  /* u(1,x)=1 */
        v[0] = 1;  /* v(1,x)=1 */
        terms[0] = u[0];   /* a(1)=1 */
        /* Terms array currently holds 1 element, capacity at least 1 */
    }

    /* Grow the sequence until it contains at least n terms */
    while (prev_len * (prev_len + 1) / 2 < n)  /* number of terms up to current row */
    {
        idx_t new_len = prev_len + 1;   /* next row length */

        /* Allocate arrays for new row */
        seq_t *u_new = (seq_t *)calloc(new_len, sizeof(seq_t));
        seq_t *v_new = (seq_t *)calloc(new_len, sizeof(seq_t));

        /* Compute the new row from the previous row */
        for (idx_t i = 0; i < prev_len; ++i)
        {
            /* u recurrence: x*u + (x+1)*v */
            u_new[i + 1] += u[i]; /* x*u term */
            u_new[i + 1] += v[i]; /* x*v term */
            u_new[i]     += v[i]; /* v term */

            /* v recurrence: x*u + v + 1 */
            v_new[i + 1] += u[i]; /* x*u term */
            v_new[i]     += v[i]; /* v term */
        }
        v_new[0] += (seq_t)1;  /* constant term of v recurrence */

        /* Ensure the terms array is large enough */
        size_t needed = prev_len * (prev_len + 1) / 2 + new_len;
        seq_t *tmp = (seq_t *)realloc(terms, needed * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failure – return 0 as a safe fallback */
            free(u_new); free(v_new); return 0;
        }
        terms = tmp;

        /* Append coefficients of the new row to the flattened sequence */
        size_t start = prev_len * (prev_len + 1) / 2;
        for (idx_t i = 0; i < new_len; ++i) {
            terms[start + i] = u_new[i];
        }

        /* Prepare for next iteration */
        free(u); free(v);
        u = u_new;
        v = v_new;
        prev_len = new_len;
    }

    /* The requested term is now guaranteed to exist */
    return terms[n - 1];
}
