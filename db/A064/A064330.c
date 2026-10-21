/*
 * A064330  Generalized Catalan numbers C(-8; n).
 * https://oeis.org/A064330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{m=0..n-1} (n-m)*binomial(n-1+m, m)*(-8)^m / n
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 14, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t sum = 0;
    seq_t binom_coeff = 1;      /* binomial(n-1+0,0) = 1   */
    seq_t pow_neg8 = 1;         /* (-8)^m  */

    for (idx_t m = 0; m < n; ++m) {
        /* term = binom_coeff * (n - m) * (-8)^m */
        seq_t term = binom_coeff * (seq_t)(n - m);
        term *= pow_neg8;

        sum += term;

        if (m + 1 < n) {
            /* Update binomial coefficient:
               binom(n-1+m+1, m+1) = binom * (n + m) / (m + 1) */
            binom_coeff = binom_coeff * (seq_t)(n + m);
            binom_coeff /= (seq_t)(m + 1);

            /* Update power of -8 for next m */
            pow_neg8 *= -8;
        }
    }

    return sum / (seq_t)n;
}
