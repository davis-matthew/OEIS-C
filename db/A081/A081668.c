/*
 * A081668  Expansion of 2*sinh(x) + BesselI_0(2*x).
 * https://oeis.org/A081668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2 when n is odd; a(2m) = binomial(2m, m) (central
 *            binomial coefficient)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n & 1ULL)
        return (seq_t)2;
    idx_t m = n >> 1;
    return binom(2 * m, m);
}
