/*
 * A153010  Indices of A153007 where the entry equals zero.
 * https://oeis.org/A153010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-1) – 1 for n > 2; a(0)=0, a(1)=1, a(2)=2, a(3)=3
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= (idx_t)3)
        return (seq_t)n;
    return pow2(n - (idx_t)1) - (seq_t)1;
}
