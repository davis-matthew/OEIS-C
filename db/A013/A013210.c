/*
 * A013210  Expansion of e.g.f.: sin(log(x+1) - sin(x)) =
 *          -1/2!*x^2+3/3!*x^3-6/4!*x^4+23/5!*x^5...
 * https://oeis.org/A013210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n!*coeff. of x^n in sin(log(1+x) - sin(x))
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 13, i64 n <= 21, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate arrays for gw, S, C */
    if (n == 0) return 0;      /* fast path for a(0) */

    /* gw[k] = w'_k * k! for k = 0..n-1 */
    seq_t *gw = NULL;
    if (n > 0) {
        gw = (seq_t*)malloc(n * sizeof(seq_t));
        gw[0] = 0;                     /* w'_0 = 0 */
        seq_t fac = 1;                 /* 0! = 1 */
        for (idx_t k = 1; k < n; ++k) {
            fac *= (seq_t)k;           /* k!    */
            seq_t term = fac;          /* (-1)^k * k!    */

            /* apply (-1)^k */
            if (k & 1) term = -term;   /* if k odd */

            /* if k is even, subtract (-1)^(k/2) */
            if ((k & 1) == 0) {
                seq_t sub = ((k/2) & 1) == 0 ? 1 : -1;
                term -= sub;
            }
            gw[k] = term;
        }
    }

    /* S and C hold a_n and cos terms, size n+1 */
    seq_t *S = (seq_t*)calloc(n + 1, sizeof(seq_t));
    seq_t *C = (seq_t*)calloc(n + 1, sizeof(seq_t));
    if (!S || !C) { free(gw); free(S); free(C); return 0; }

    S[0] = 0;     /* a(0) = 0 */
    C[0] = 1;     /* cos(G(0)) = 1 */

    for (idx_t m = 0; m < n; ++m) {
        seq_t sumS = 0;
        seq_t sumC = 0;
        seq_t bin  = 1;                 /* binom(m, 0) */

        for (idx_t i = 0; i <= m; ++i) {
            seq_t gw_val  = gw[m - i];
            seq_t termS   = bin * C[i] * gw_val;
            sumS         += termS;
            seq_t termC   = bin * S[i] * gw_val;
            sumC         += termC;

            if (i < m) {
                /* update binom(m, i+1) = binom(m, i) * (m-i)/(i+1) */
                seq_t mult = (seq_t)(m - i);
                seq_t div  = (seq_t)(i + 1);
                bin = (bin * mult) / div;
            }
        }

        S[m + 1] = sumS;
        C[m + 1] = -sumC;
    }

    seq_t result = S[n];
    free(S); free(C); free(gw);
    return result;
}
