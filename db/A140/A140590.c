/*
 * A140590  Exchange successive pairs of terms of A000051.
 * https://oeis.org/A140590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n + (-1)^n) + 1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* exponent = n+1 for even n, n-1 for odd n */
    idx_t e = (n & 1ULL) ? n - 1 : n + 1;
    return pow_uint(2, e) + 1;
}
