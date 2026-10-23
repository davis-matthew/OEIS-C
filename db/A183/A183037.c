/*
 * A183037  a(n) = A001511(n)*2^A001511(n) where A001511(n) equals the 2-adic
 *          valuation of 2n.
 * https://oeis.org/A183037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1+v2(n))*2^(1+v2(n))
 * Range:     u8 n <= 31, u16 n <= 4095, u32 n <= 9625, u64 n <= 9625,
 *            u128 n <= 9625, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 1 + v2(n), the 2-adic valuation of 2n. */
    idx_t t = n;
    idx_t tz = 0;
    while ((t & 1) == 0) { /* n is even */
        ++tz;
        t >>= 1;
    }
    /* v2(2n) = 1 + v2(n) */
    idx_t k = tz + 1;

    /* 2^k as a seq_t */
    seq_t power = pow2(k);

    /* Return k * 2^k */
    return (seq_t)k * power;
}
