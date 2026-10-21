/*
 * A097689  Number of prime pairs (p,q) with p^2 + q^2 <= n^2.
 * https://oeis.org/A097689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 1823, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, count_le
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = number of ordered prime pairs (p,q) with p² + q² ≤ n² */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/build_primes.h"   /* provides build_primes */
#include "../utils/count_le.h"

static idx_t *primes = NULL;      /* array of primes */
static size_t primes_cnt = 0;     /* number of primes stored */
static idx_t prime_limit = 0;     /* largest prime computed so far */

/* Integer square root (floor). Uses __int128 to avoid overflow when
   multiplying mid * mid on 64‑bit machines. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x, res = 0;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        __int128 sq = (__int128)mid * mid;
        if (sq <= x) { res = mid; lo = mid + 1; }
        else           hi = mid - 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* a(1) = 0 */

    /* Rebuild the prime list if we need primes larger than the current limit. */
    if (prime_limit < n) {
        free(primes);
        build_primes(n, &primes, &primes_cnt);
        prime_limit = n;
    }

    seq_t result = 0;
    idx_t n_sq = n * n;

    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];
        if (p > n) break;            /* p² alone would exceed n² */

        idx_t limit_sq = n_sq - p * p;
        idx_t q_max    = isqrt_u64(limit_sq);

        size_t count = count_le(primes, primes_cnt, q_max);
        result += (seq_t)count;
    }

    return result;
}
