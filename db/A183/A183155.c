/*
 * A183155  The number of order-preserving partial isometries (of an n-chain)
 *          of fix zero (fix of alpha = 0). Equivalently, the number of
 *          order-preserving partial derangement isometries (of an n-chain).
 * https://oeis.org/A183155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1) - (2*n + 1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t pow = pow2(n + 1);          /* compute 2^(n+1) */
    seq_t sub = (seq_t)2 * (seq_t)n + 1; /* compute 2*n + 1 */
    return pow - sub;
}
