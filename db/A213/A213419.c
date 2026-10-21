/*
 * A213419  Expansion of q * chi(-q) / chi(-q^25) in powers of q where chi()
 *          is a Ramanujan theta function.
 * https://oeis.org/A213419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in x * ∏_{k>0} (1 + x^(25k))/(1 + x^k)
 * Range:     i8 n <= 56, i16 n <= 156, i32 n <= 483, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code uses dynamic programming to form the series of the
   reciprocal of ∏(1 + x^k) and the series of ∏(1 + x^(25k)),
   then multiplies them.  The numerator is built with a backward
   loop to avoid using the same factor more than once. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset is 1 */

    const idx_t N = n;                         /* we need coefficients up to x^(n‑1) */
    const size_t sz = (size_t)(N + 1);         /* array size: indices 0 … N */

    seq_t *denom = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *num   = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *conv  = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!denom || !num || !conv) {
        free(denom); free(num); free(conv);
        return 0;                            /* memory allocation failure */
    }

    /* 1.  Build the series for 1/∏(1 + x^k)  */
    denom[0] = 1;                             /* constant term */
    for (idx_t k = 1; k <= N; ++k) {
        for (idx_t i = k; i <= N; ++i) {
            denom[i] -= denom[i - k];
        }
    }

    /* 2.  Build the series for ∏(1 + x^(25k))  */
    num[0] = 1;                               /* empty product */
    for (idx_t k = 1; 25 * k <= N; ++k) {
        const idx_t step = 25 * k;
        /* Use a backward loop to ensure each factor is used at most once */
        for (idx_t i = N; ; --i) {
            num[i] += num[i - step];
            if (i == step) break;             /* stop after processing step */
        }
    }

    /* 3.  Convolution of num and denom to obtain the series for the ratio */
    for (idx_t i = 0; i <= N; ++i) {
        seq_t sum = 0;
        for (idx_t j = 0; j <= i; ++j) {
            sum += num[j] * denom[i - j];
        }
        conv[i] = sum;
    }

    seq_t result = conv[n - 1];                /* coefficient of x^(n‑1) */

    free(denom); free(num); free(conv);
    return result;
}
