/*
 * A098660  E.g.f. BesselI(0,2*sqrt(2)*x) + BesselI(1,2*sqrt(2)*x)/sqrt(2).
 * https://oeis.org/A098660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n, floor(n/2)) * 2^floor(n/2)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 45, u128 n <= 87,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* floor(n/2) */
    idx_t k = n >> 1;

    /* binomial coefficient C(n, k) */
    seq_t bin = binomial(n, k);

    /* compute 2^k */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < k; ++i) {
        pow2 *= (seq_t)2;
    }

    /* result */
    return bin * pow2;
}
