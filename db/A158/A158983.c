/*
 * A158983  Coefficients of polynomials (in descending powers of x) P(n,x) :=
 *          2 + P(n-1,x)^2, where P(1,x) = x + 2.
 * https://oeis.org/A158983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A158983 – coefficients of P(n,x) in descending powers.
   P(1,x) = x + 2,
   P(k,x) = 2 + P(k‑1,x)^2   (k ≥ 2).  */ 

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1 */

    /* current polynomial coefficients in ascending degree order:
       coeff[0] is constant term, coeff[deg] is leading coefficient (1)   */
    idx_t deg = 1;                 /* degree of row 1 */
    seq_t *coeff = (seq_t *)malloc((deg + 1) * sizeof(seq_t));
    coeff[0] = 2;                  /* constant term  */
    coeff[1] = 1;                  /* coefficient of x^1        */

    idx_t counted = 0;             /* number of terms output so far */

    while (1) {
        idx_t sz = deg + 1;          /* number of coefficients in this row */

        if (n <= counted + sz) {    /* the requested term is in this row */
            /* position inside the row (1‑based) */
            idx_t pos = n - counted;        /* 1 <= pos <= sz */
            /* coefficient in descending order */
            idx_t idx_in_poly = deg - (pos - 1);
            seq_t result = coeff[idx_in_poly];
            free(coeff);
            return result;
        }

        counted += sz;              /* skip this entire row    */

        /* compute next polynomial (square plus 2) */
        idx_t next_deg = 2 * deg;
        idx_t next_sz  = next_deg + 1;
        seq_t *next = (seq_t *)malloc(next_sz * sizeof(seq_t));

        /* initialize to zero */
        for (idx_t i = 0; i < next_sz; ++i)
            next[i] = 0;

        /* convolution: next[i+j] += coeff[i] * coeff[j] */
        for (idx_t i = 0; i <= deg; ++i) {
            for (idx_t j = 0; j <= deg; ++j) {
                next[i + j] += coeff[i] * coeff[j];
            }
        }

        /* add the constant 2 to the constant term */
        next[0] += 2;

        free(coeff);
        coeff = next;
        deg = next_deg;
    }
}
