/*
 * A135982  a(n) = 2^(24*n+7)-1.
 * https://oeis.org/A135982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(24*n+7)-1
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t e = (idx_t)24 * n + (idx_t)7;
    return pow2(e) - (seq_t)1;
}
