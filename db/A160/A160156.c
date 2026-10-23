/*
 * A160156  Partial sums of A007583.
 * https://oeis.org/A160156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (3*n + 1 + 2^(2*n+3))/9
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t val = ((seq_t)n * (seq_t)3) + (seq_t)1 + pow2(2*n + 3);
    return val / (seq_t)9;
}
