/*
 * A013905  a(n) = 22^(5n+4).
 * https://oeis.org/A013905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22^(5*n+4)
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    idx_t exp = 5*n + 4;
    return pow_uint(22, exp);
}
