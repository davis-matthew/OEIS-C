/*
 * A066791  a(n) = phi(n^2 + n + 1).
 * https://oeis.org/A066791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = phi(n^2 + n + 1)
 * Range:     u8 n <= 15, u16 n <= 258, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    /* Use a wider integer to avoid overflow when computing n^2 + n + 1. */
    __uint128_t nn   = n;
    __uint128_t m128 = nn * (nn + 1) + 1;
    seq_t m = (seq_t)m128;
    return calc_phi(m);
}
