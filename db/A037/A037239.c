/*
 * A037239  Numerator of Pi^(2n)/(Gamma(2n)*(1-2^(-2n))*Zeta(2n)); =
 *          8*(highest power of 2 dividing n).
 * https://oeis.org/A037239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 8 * (highest power of 2 dividing n) = 2^(v2(n)+3)
 * Range:     u8 n <= 31, u16 n <= 8191, u32 n <= 9197, u64 n <= 9197,
 *            u128 n <= 9197, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* Out of offset, harmless */

    /* Count trailing zeros in n (i.e., v2(n)) */
    idx_t t = n;
    uint32_t tz = 0;
    while ((t & 1) == 0) {
        tz++;
        t >>= 1;
    }

    /* Compute 2^(tz+3) */
    seq_t res = (seq_t)1;
    res <<= (tz + 3);

    return res;
}
