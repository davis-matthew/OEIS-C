/*
 * A020540  a(n) = 8^(n+1) - 2^(n+2).
 * https://oeis.org/A020540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8^(n+1) - 2^(n+2)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 8^(n+1) is 2^(3*(n+1)) */
    idx_t hi_exp = 3 * (n + 1);
    seq_t high = pow2(hi_exp);
    seq_t low  = pow2(n + 2);
    return high - low;
}
