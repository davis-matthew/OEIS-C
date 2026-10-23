/*
 * A046161  a(n) = denominator of binomial(2n,n)/4^n.
 * https://oeis.org/A046161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, frac
 * Formula:   a(n) = 2^(2n - popcount(n)) (popcount is number of 1‑bits in n)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, a(1) = 2, a(2) = 8, a(3) = 16, a(4) = 128, ... */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute popcount of n */
    unsigned int pc = 0;
    idx_t t = n;
    while (t) {
        t &= t - 1;
        ++pc;
    }

    /* exponent for power of two */
    idx_t exp = 2 * n - pc;
    return pow2(exp);
}
