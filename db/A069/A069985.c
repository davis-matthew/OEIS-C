/*
 * A069985  Numerator of b(n) = binomial(2n,n)^3*(42n+5)/2^(12n+4).
 * https://oeis.org/A069985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = numerator of binomial(2n,n)^3 * (42n+5) / 2^(12n+4)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* binomial(2n, n) */
    seq_t bin = binomial(2 * n, n);

    /* 2-adic valuation of the binomial coefficient */
    idx_t v2_bin = 0;
    seq_t tmp = bin;
    while ((tmp & 1) == 0)
    {
        tmp >>= 1;
        v2_bin++;
    }

    /* remove all 2s from binomial before cubing */
    seq_t bin_div = bin >> v2_bin;

    /* cube bin_div */
    seq_t num = bin_div;
    num = num * bin_div;
    num = num * bin_div;

    /* multiply by (42n+5) */
    seq_t factor = (seq_t)42 * n + 5;
    num = num * factor;

    return num;
}
