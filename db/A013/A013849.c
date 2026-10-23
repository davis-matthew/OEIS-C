/*
 * A013849  a(n) = 8^(5*n + 4).
 * https://oeis.org/A013849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^(5*n + 4)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 7,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 8^(5*n + 4) == 2^(3*(5*n + 4)) == 2^(15*n + 12) */
    idx_t exp = 15 * n + 12;
    return pow2(exp);
}
