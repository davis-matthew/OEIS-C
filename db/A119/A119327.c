/*
 * A119327  Expansion of (1-4x+12x^2-16x^3+8x^4)/(1-x)^5.
 * https://oeis.org/A119327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum{j=0..n} C(4,2j) * C(n,2j)
 * Range:     u8 n <= 7, u16 n <= 26, u32 n <= 475, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simplified: a(n) = 1 + 6 * Bin(n,2) + Bin(n,4) */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t res = 1;   /* j = 0  term: C(4,0)*C(n,0) = 1 */
    if (n >= 2) {
        seq_t c2 = binom(n, 2);
        res += 6 * c2;
    }
    if (n >= 4) {
        seq_t c4 = binom(n, 4);
        res += c4;
    }
    return res;
}
