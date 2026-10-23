/*
 * A110695  Kekulé numbers for certain benzenoids of trigonal symmetry.
 * https://oeis.org/A110695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*binom(2*n,n) - 1) * (binom(2*n,n)^2 - binom(2*n,n) +
 *            1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t C = binom(2 * n, n);
    seq_t t1 = ((seq_t)2 * C) - (seq_t)1;
    seq_t t2 = C * C - C + (seq_t)1;
    return t1 * t2;
}
