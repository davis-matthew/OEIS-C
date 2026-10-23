/*
 * A160587  Numerator of Laguerre(n, -10).
 * https://oeis.org/A160587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Laguerre(n, -10)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 35,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;

    /* L_0 = 1/1, L_1 = 11/1 */
    seq_t num_prev2 = (seq_t)1, den_prev2 = (seq_t)1;
    seq_t num_prev1 = (seq_t)11, den_prev1 = (seq_t)1;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t C = (seq_t)(2 * i + 9);        /* 2i + 9, since x = -10 */
        /* Terms: C * L_{i-1} - (i-1) * L_{i-2} */
        seq_t t1_num = C * num_prev1;       /* numerator part of first term */
        seq_t t1_den = den_prev1;           /* denominator part of first term */

        seq_t t2_num = (i - 1) * num_prev2;/* numerator part of second term */
        seq_t t2_den = den_prev2;           /* denominator part of second term */

        /* Compute lcm of denominators to subtract safely */
        seq_t g = seq_gcd(t1_den, t2_den);
        seq_t lcm_den = (t1_den / g) * t2_den;   /* lcm(t1_den, t2_den) */

        seq_t factor1 = lcm_den / t1_den;      /* corresponds to t2_den / g */
        seq_t factor2 = lcm_den / t2_den;      /* corresponds to t1_den / g */

        seq_t new_num = t1_num * factor1 - t2_num * factor2;
        seq_t new_den = lcm_den * (seq_t)i;    /* divide by i at the end */

        /* Reduce the fraction to keep numbers small */
        seq_t g2 = seq_gcd(new_num, new_den);
        new_num /= g2;
        new_den /= g2;

        /* Slide window of last two values */
        num_prev2 = num_prev1; den_prev2 = den_prev1;
        num_prev1 = new_num;   den_prev1 = new_den;
    }

    return num_prev1;   /* Numerator of L_n(-10) in lowest terms */
}
