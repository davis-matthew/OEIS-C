/*
 * A238015  Denominator of (2*n+1)!*8*Bernoulli(2*n,1/2).
 * https://oeis.org/A238015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 2^max(0, popcount(2*n+1) - 4)
 * Range:     u8 n <= 2000, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t m = (idx_t)2 * n + (idx_t)1;          /* m = 2n + 1          */
    int bits = __builtin_popcountll((unsigned long long)m);
    int exp  = bits - 4;                        /* exponent of 2       */
    if (exp <= 0)
        return (seq_t)1;
    return pow2((idx_t)exp);
}
