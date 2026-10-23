/*
 * A152551  a(n) = (2*n+1)^floor((n-1)/2).
 * https://oeis.org/A152551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n+1)^floor((n-1)/2)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 41,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    return pow_uint(2 * n + 1, (n - 1) / 2);
}
