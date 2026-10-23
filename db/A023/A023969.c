/*
 * A023969  a(n) = round(sqrt(n)) - floor(sqrt(n)).
 * https://oeis.org/A023969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(sqrt(n)) - floor(sqrt(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    /* f = floor(sqrt(n)) */
    idx_t f = _floor_isqrt_u64(n);

    /* square of f (may require 128‑bit to avoid overflow) */
    idx_t sq = (idx_t)((unsigned __int128)f * (unsigned __int128)f);

    /* threshold: f^2 + f + 1 */
    idx_t t = sq + f + 1;

    return (n >= t) ? (seq_t)1 : (seq_t)0;
}
