/*
 * A093280  a(n) is the largest number such that all of a(n)'s length-n
 *          substrings are distinct and divisible by 80.
 * https://oeis.org/A093280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 0, a(2) = 800, a(3) = 88000, a(n) = 10^(2n-1) − 8·10^n
 *            for n ≥ 4
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)800;
    if (n == 3)
        return (seq_t)88000;

    /* Compute 10^n and 10^(n-1) */
    seq_t pow_n        = pow10((idx_t)n);
    seq_t pow_n_minus1 = pow10((idx_t)(n - 1));

    /* 10^(2n-1) = 10^n * 10^(n-1) */
    seq_t pow_2n_minus1 = pow_n * pow_n_minus1;

    /* 8 * 10^n */
    seq_t eight_times_pow_n = (seq_t)8 * pow_n;

    /* a(n) = 10^(2n-1) - 8*10^n */
    return pow_2n_minus1 - eight_times_pow_n;
}
