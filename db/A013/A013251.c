/*
 * A013251  Expansion of e.g.f. sinh(log(x+1) - arctan(x)).
 * https://oeis.org/A013251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n/n! in the expansion of sinh(log(1+x) -
 *            arctan(x))
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* a(0) = 0 */

    /* Allocate temporary arrays */
    seq_t *fact = (seq_t*)malloc((n + 1) * sizeof(seq_t));   /* factorials */
    seq_t *r    = (seq_t*)malloc(n * sizeof(seq_t));         /* ordinary coeffs of f' */
    seq_t *f    = (seq_t*)malloc((n + 1) * sizeof(seq_t));   /* EGF coeffs of F(x) */
    seq_t *y    = (seq_t*)malloc((n + 1) * sizeof(seq_t));   /* G(x) = sinh(F) coeffs */
    seq_t *c    = (seq_t*)malloc((n + 1) * sizeof(seq_t));   /* cosh(F) coeffs */

    /* factorials */
    fact[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* r(k) = coefficient of x^k in derivative f' */
    for (idx_t k = 0; k < n; ++k) {
        int sign1 = ((k & 1) == 0) ? 1 : -1;          /* (-1)^k */
        if ((k & 1) == 0) {                         /* even k */
            int sign2 = ((((k >> 1) & 1) == 0) ? 1 : -1); /* (-1)^(k/2) */
            r[k] = sign1 - sign2;
        } else {                                     /* odd k */
            r[k] = sign1;
        }
    }

    /* EGF coefficients of F(x) : f[n] = (n-1)! * r[n-1]  for n>=1, f[0]=0 */
    f[0] = 0;
    for (idx_t m = 1; m <= n; ++m)
        f[m] = fact[m - 1] * r[m - 1];

    /* Initialize G and cosh(F) */
    y[0] = 0;
    c[0] = 1;

    /* Recurrence derived from y' = c * f' and c' = y * f' */
    for (idx_t m = 0; m < n; ++m) {
        seq_t binom = 1;         /* C(m,0) */
        seq_t sum_y = 0;
        seq_t sum_c = 0;

        for (idx_t k = 0; k <= m; ++k) {
            /* term for y[m+1] */
            seq_t term = binom * c[k];
            term = term * f[m - k + 1];
            sum_y += term;

            /* term for c[m+1] */
            term = binom * y[k];
            term = term * f[m - k + 1];
            sum_c += term;

            /* update binomial coefficient for next k */
            if (k < m) {
                seq_t num = binom * (seq_t)(m - k);
                seq_t den = (seq_t)(k + 1);
                binom = num / den;   /* exact division */
            }
        }

        y[m + 1] = sum_y;
        c[m + 1] = sum_c;
    }

    seq_t result = y[n];

    /* Free temporary memory */
    free(fact);
    free(r);
    free(f);
    free(y);
    free(c);

    return result;
}
