/*
 * A198797  a(n) = 4*6^n-1.
 * https://oeis.org/A198797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*6^n - 1 (OEIS A198797)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* For standard fixed-width integer types if needed */

seq_t a(idx_t n)
{
    /* Base case, a(0) = 3 */
    if (n == 0) {
        return (seq_t)3;
    }

    /* Compute 6^n iteratively */
    seq_t pow6 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        pow6 *= (seq_t)6;
    }

    /* a(n) = 4 * 6^n - 1 */
    seq_t result = pow6 * (seq_t)4;
    result -= (seq_t)1;
    return result;
}
