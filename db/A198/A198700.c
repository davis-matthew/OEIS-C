/*
 * A198700  a(n) = 11*10^n - 1.
 * https://oeis.org/A198700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*10^n - 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)10;
    }

    seq_t val = (seq_t)10; /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        /* a(i) = 10*a(i-1) + 9 */
        val = mul_small(val, 10) + (seq_t)9;
    }
    return val;
}
