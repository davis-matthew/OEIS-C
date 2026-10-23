/*
 * A022670  Expansion of Product_{m >= 1} (1-m*q^m)^10.
 * https://oeis.org/A022670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   Expansion of Product_{m >= 1} (1-m*q^m)^10
 * Range:     i8 n <= 3, i16 n <= 11, i32 n <= 33, i64 n <= 90, i128 n <= 229,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Allocate arrays for the current coefficient set and a temporary buffer.
       The arrays have size n+1 to hold coefficients from 0 to n. */
    seq_t *coeff = (seq_t *)calloc(n + 1, sizeof(seq_t));
    seq_t *temp  = (seq_t *)calloc(n + 1, sizeof(seq_t));
    if (!coeff || !temp) {
        /* Allocation failure – return zero.  In normal use this is never
           expected to happen for n <= 10000. */
        free(coeff);
        free(temp);
        return (seq_t)0;
    }

    coeff[0] = (seq_t)1;  /* Start with G = 1 */

    /* Binomial coefficients for (1 - x)^10 */
    const int bin[11] = {
        1, 10, 45, 120, 210, 252, 210, 120, 45, 10, 1
    };

    /* Sequentially multiply by (1 - m*q^m)^10 for m = 1 .. n. */
    for (idx_t m = 1; m <= n; ++m) {
        /* Reset temporary buffer. */
        for (idx_t i = 0; i <= n; ++i)
            temp[i] = (seq_t)0;

        /* Compute coefficients of (1 - m*q^m)^10. */
        seq_t term[11];
        term[0] = (seq_t)1;            /* coefficient of q^(0) */
        seq_t pow_m = (seq_t)m;        /* m^1 */
        for (int k = 1; k <= 10; ++k) {
            seq_t val = pow_m * (seq_t)bin[k];
            if (k & 1)                 /* sign alternates */
                val = -val;
            term[k] = val;
            pow_m = pow_m * (seq_t)m;   /* prepare m^(k+1) */
        }

        /* Convolution: coeff (*) term -> temp */
        for (idx_t j = 0; j <= n; ++j) {
            seq_t cj = coeff[j];
            if (cj == (seq_t)0)
                continue;
            for (int k = 0; k <= 10; ++k) {
                idx_t idx = j + m * (idx_t)k;
                if (idx > n)
                    break;
                temp[idx] += cj * term[k];
            }
        }

        /* Swap coeff and temp for next iteration. */
        seq_t *swap = coeff;
        coeff = temp;
        temp = swap;
    }

    /* Result is coefficient of q^n. */
    seq_t result = coeff[n];

    free(coeff);
    free(temp);
    return result;
}
