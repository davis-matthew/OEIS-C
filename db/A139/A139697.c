/*
 * A139697  Binomial transform of [1, 12, 12, 12, ...].
 * https://oeis.org/A139697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*2^(n-1)-11
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n must be ≥ 1 (offset 1).  Behaviour for n < 1 is irrelevant. */
    seq_t v = pow2(n - 1);   /* 2^(n-1) */
    v = v * 12;              /* 12 * 2^(n-1) */
    v -= 11;                 /* subtract 11 */
    return v;
}
