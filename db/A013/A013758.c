/*
 * A013758  a(n) = 16^(3n+1).
 * https://oeis.org/A013758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16^(3n+1)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 4, u128 n <= 10,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 16^(3n+1) = 2^(4*(3n+1)) = 2^(12n+4) */
    idx_t exp = 12 * n + 4;
    return pow2(exp);
}
