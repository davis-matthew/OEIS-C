/*
 * A064331  Generalized Catalan numbers C(-9; n).
 * https://oeis.org/A064331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{m=0}^{n-1} ((n-m)*binomial(n-1+m, m)*(-9)^m)/n with
 *            a(0) = 1
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 14, i128 n <= 27,
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

    seq_t sum   = (seq_t)0;
    seq_t binom = (seq_t)1;   /* binomial(n-1+0,0) = 1 */
    seq_t power = (seq_t)1;   /* (-9)^0 = 1 */

    for (idx_t m = 0; m < n; ++m) {
        /* term = ((n-m)*binom / n) * power; each step is integer */
        seq_t term = ((seq_t)(n - m) * binom / (seq_t)n) * power;
        sum += term;

        if (m + 1 < n) {
            /* update binom: binom(n-1+(m+1), m+1) = binom * (n-1+m+1) / (m+1) */
            binom = binom * (seq_t)(n - 1 + m + 1) / (seq_t)(m + 1);
            /* update power: (-9)^(m+1) = power * (-9) */
            power = power * (seq_t)(-9);
        }
    }

    return sum;
}
