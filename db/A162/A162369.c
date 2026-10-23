/*
 * A162369  Number of reduced words of length n in the Weyl group D_27.
 * https://oeis.org/A162369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in (1 + x + … + x^26) * ∏_{i=1}^{26}
 *            (1 + x + … + x^{2i-1})
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 46, u128 n <= 702,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the coefficients of the growth series of the
   Weyl group D_27 once, and return the requested value.
   The coefficients beyond the degree of the polynomial are zero. */

#include <stddef.h>
#include <stdlib.h>   /* for malloc */

seq_t a(idx_t n)
{
    /* static state to avoid recomputing the polynomial */
    static seq_t *coeffs = NULL;   /* array of coefficients */
    static idx_t degree = 0;       /* highest degree with non‑zero coefficient */

    /* Build the polynomial on first call */
    if (coeffs == NULL) {
        /* Start with f(27) = 1 + x + … + x^26 */
        idx_t maxdeg = 26;                 /* current maximum degree */
        seq_t *res = (seq_t*)malloc((maxdeg + 1) * sizeof(seq_t));
        if (res == NULL) return 0;          /* allocation failed → zero */
        for (idx_t i = 0; i <= maxdeg; ++i)
            res[i] = 1;                      /* coefficient 1 for each power */

        /* Multiply by f(2i) for i = 1 .. 26 */
        for (idx_t i = 1; i <= 26; ++i) {
            /* f(2i) has coefficients 1 for powers 0 .. (2*i-1) */
            idx_t m = 2 * i;                 /* length of f(2i) = 2*i */
            idx_t new_deg = maxdeg + (m - 1);
            seq_t *new_res = (seq_t*)malloc((new_deg + 1) * sizeof(seq_t));
            if (new_res == NULL) {
                /* Allocation failed: free and abort, return zero */
                free(res);
                return 0;
            }
            for (idx_t j = 0; j <= new_deg; ++j)
                new_res[j] = 0;               /* initialize to zero */

            /* Convolution: since all coefficients of f(2i) are 1
               we can add res[a] to new_res[a+b] for each a,b. */
            for (idx_t a = 0; a <= maxdeg; ++a) {
                seq_t val = res[a];
                if (val == 0) continue;
                for (idx_t b = 0; b < m; ++b) {
                    new_res[a + b] += val;
                }
            }
            free(res);
            res = new_res;
            maxdeg = new_deg;
        }
        coeffs = res;
        degree = maxdeg;
    }

    /* Return the requested coefficient, or zero if out of bounds */
    if (n <= degree)
        return coeffs[n];
    else
        return 0;
}
