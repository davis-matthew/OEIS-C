/*
 * A100103  a(n) = 2^(2*n) - 2*n.
 * https://oeis.org/A100103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2*n) - 2*n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   pow2exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2exp.h"

seq_t a(idx_t n)
{
    /* Compute 4^n = (2^n)^2, then subtract 2*n */
    seq_t pown  = pow2exp(n);            /* 2^n */
    seq_t pow2n = pown * pown;           /* 4^n */
    return pow2n - ((seq_t)n * 2);
}
