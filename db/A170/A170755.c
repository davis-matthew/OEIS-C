/*
 * A170755  Expansion of g.f.: (1+x)/(1-35*x).
 * https://oeis.org/A170755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(n)=36*35^(n-1) for n>0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0U)
        return (seq_t)1;
    seq_t pow35 = big_pow((seq_t)35, (seq_t)(n - 1U));
    return pow35 * (seq_t)36;
}
