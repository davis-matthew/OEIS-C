/*
 * A196305  a(n) = 15*2^n - 1.
 * https://oeis.org/A196305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*2^n - 1
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    seq_t pow = pow2(n);            /* 2^n   */
    seq_t prod = mul_small(pow, 15u);  /* 15 * 2^n */
    return prod - 1;
}
