/*
 * A038165  G.f.: 1/((1-x)*(1-x^2))^5.
 * https://oeis.org/A038165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n/2 + 7, 7) * (4*(n/2)^2 + (n even ? 23*(n/2) +
 *            18 : 41*(n/2) + 90)) / 18
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* compute k = floor(n/2) */
    idx_t k = n >> 1;

    /* binomial part */
    seq_t bin = binom(k + 7, 7);          /* binomial(k+7, 7) */

    /* polynomial part */
    seq_t kseq = (seq_t)k;
    seq_t num;
    if ((n & 1) == 0)                    /* even n = 2*k */
    {
        num = 4 * kseq * kseq + 23 * kseq + 18;
    }
    else                                 /* odd n = 2*k+1 */
    {
        num = 4 * kseq * kseq + 41 * kseq + 90;
    }

    /* final result */
    return (bin * num) / 18;
}
