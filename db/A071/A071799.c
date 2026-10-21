/*
 * A071799  Number of lattice paths in the lattice [0..2n] X [0..2n] which do
 *          not pass through the point (n,n).
 * https://oeis.org/A071799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = binomial(4*n, 2*n) - binomial(2*n, n)^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   binomial, binomial_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include <stdbool.h>
#include "../utils/binomial_uint128.h"

/* Helper for the C implementation (idx_t is unsigned 64‑bit integer). */
#ifndef __cplusplus

#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safety guard – offset starts at 1 */

#ifdef __cplusplus
    /* Big‑integer build – use the provided binomial and arithmetic. */
    seq_t binom1 = binomial((idx_t)(4 * n), (idx_t)(2 * n));
    seq_t binom2 = binomial((idx_t)(2 * n), n);
    seq_t sq    = binom2 * binom2;
    return binom1 - sq;
#else
    /* Arithmetic build – all computations stay within ⟨unsigned⟩128 bits. */
    idx_t two_n = 2 * n;
    idx_t four_n = 4 * n;

    seq_t binom1 = binomial_uint128(four_n, two_n);
    seq_t binom2 = binomial_uint128(two_n, n);

    __uint128_t sq = (__uint128_t)binom2 * (__uint128_t)binom2;
    __int128_t diff = ( __int128_t)binom1 - (__int128_t)sq;
    return (seq_t)diff;
#endif
}
