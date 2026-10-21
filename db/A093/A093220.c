/*
 * A093220  a(n) is the largest number such that all of a(n)'s length-n
 *          substrings are distinct and divisible by 20.
 * https://oeis.org/A093220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 0, for n ≥ 2 a(n) = 10^n · (10^(n−1) − 2) (equivalently,
 *            a(n) consists of n−2 digits 9, an 8, and n zeros)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)0;
    }

    /* Compute 10^n and 10^(n-1) in seq_t arithmetic. */
    seq_t pow10_n = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        pow10_n *= 10;
    }

    seq_t pow10_nm1 = (seq_t)1;
    for (idx_t i = 0; i < n - 1; ++i) {
        pow10_nm1 *= 10;
    }

    /* a(n) = 10^n * (10^(n-1) - 2) */
    return pow10_n * (pow10_nm1 - 2);
}
