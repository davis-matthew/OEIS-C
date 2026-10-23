/*
 * A116362  Smallest m such that A116361(m) = n.
 * https://oeis.org/A116362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(n)=2^(n-1)+2^floor((n+1)/2)-1 for n>1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    seq_t t1 = pow2(n - 1);
    seq_t t2 = pow2((n + 1) / 2);
    return t1 + t2 - 1;
}
