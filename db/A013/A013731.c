/*
 * A013731  a(n) = 2^(3*n+2).
 * https://oeis.org/A013731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(3*n+2)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    return pow_uint(2ULL, 3*n + 2);
}
