/*
 * A171494  a(n) = 2*a(n-1) for n > 1; a(0) = 6, a(1) = 16.
 * https://oeis.org/A171494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) for n > 1; a(0) = 6, a(1) = 16
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)6;
    /* For n >= 1, a(n) = 2^(n+3) */
    return pow2(n + 3);
}
