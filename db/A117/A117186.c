/*
 * A117186  Expansion of (1+x)c(x^2)/((1-xc(x^2))*sqrt(1-4x^2)), c(x) the g.f.
 *          of A000108.
 * https://oeis.org/A117186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (3*2^n - binomial(2*floor((n+1)/2), floor((n+1)/2)))/2
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* m = floor((n+1)/2) */
    idx_t m = (n + 1) >> 1;
    /* two_n = 2^n  using seq_pow */
    seq_t two_n = seq_pow((seq_t)2, n);
    /* term1 = 3 * 2^n */
    seq_t term1 = (seq_t)3 * two_n;
    /* bin = binomial(2*m, m) */
    seq_t bin = binomial((idx_t)(2 * m), m);
    /* diff = term1 - bin */
    seq_t diff = term1 - bin;
    /* return diff / 2 */
    return diff / 2;
}
