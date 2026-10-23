/*
 * A036095  Centered cube numbers: a(n) = (n+1)^17 + n^17.
 * https://oeis.org/A036095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^17 + n^17
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 12, u128 n <= 176,
 *            big any n
 * Helpers:   pow17
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow17.h"

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t term1 = pow17(nseq + (seq_t)1);
    seq_t term2 = pow17(nseq);
    return term1 + term2;
}
