/*
 * A174321  Index of the smallest prime greater than (6n+1)^2.
 * https://oeis.org/A174321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime_pi((6*n+1)^2) + 1
 * Range:     u8 n <= 6, u16 n <= 150, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the index (1‑based) of the smallest prime greater than (6n+1)^2. */

#include <stdint.h>
#include "../utils/prime_pi.h"

/* seq_t and idx_t are defined by the build environment. */
seq_t a(idx_t n)
{
    /* 6*n+1 fits in idx_t for all practical n (≤10000). */
    idx_t t  = 6 * n + 1;
    idx_t sq = t * t;                 /* (6n+1)^2 */

    /* Count of primes ≤ (6n+1)^2. */
    seq_t pi = prime_pi(sq);

    /* The smallest prime index greater than sq is pi+1. */
    return pi + 1;
}
