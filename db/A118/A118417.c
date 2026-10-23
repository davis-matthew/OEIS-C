/*
 * A118417  a(n) = (2*n + 1) * 2^(n + 1).
 * https://oeis.org/A118417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n + 1) * 2^(n + 1)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 25, u64 n <= 56, u128 n <= 119,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t k = (seq_t)n * 2 + 1;    /* (2n + 1) */
    seq_t p = pow2(n + 1);         /* 2^(n+1) */
    return k * p;
}
