/*
 * A166684  Numbers n such that d(n)<4.
 * https://oeis.org/A166684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 6541, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A166684: 1, every prime, and every square of a prime,
   written in increasing order. */

#include <stdint.h>
#include <stdlib.h>

/* Utilities for generating primes up to a limit */
#include "../utils/build_primes_upto.h"

/* Prime storage */
static uint64_t *primes      = NULL;   /* Array of primes in ascending order */
static size_t   primes_cnt   = 0;      /* Number of primes stored          */
static idx_t    primes_lim   = 0;      /* Highest value for which primes
                                          have been generated              */

/* Ensure all primes ≤ limit are available. */
static void
ensure_primes(idx_t limit)
{
    if (limit <= primes_lim)
        return;                /* Already sufficient */

    if (primes)
        free(primes);          /* Dispose old list */

    build_primes_upto(limit, &primes, &primes_cnt);
    primes_lim = limit;
}

/* Integer square root ⌊√n⌋ for 64‑bit unsigned integers. */
static inline idx_t
floor_sqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = ((idx_t)1 << 32);
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Count how many primes ≤ bound. */
static inline size_t
count_primes_leq(idx_t bound)
{
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (primes[mid] <= bound)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;                 /* number of primes ≤ bound */
}

/* Sequence generator: a(n) for n ≥ 1. */
seq_t
a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;      /* The first term is 1 */

    /* Choose an initial bound large enough to contain the n‑th term. */
    idx_t bound = n * 20ULL + 100ULL;
    if (bound < 10ULL)
        bound = 10ULL;

    /* Increase bound until we are sure the n‑th term lies <= bound. */
    while (1) {
        ensure_primes(bound);

        idx_t sqrt_bound = floor_sqrt_u64(bound);
        size_t sq_cnt = count_primes_leq(sqrt_bound);

        size_t total = 1 + primes_cnt + sq_cnt; /* plus the number 1 */
        if (total >= n)
            break;               /* bound is sufficient */

        bound *= 2ULL;            /* grow bound and retry */
    }

    /* Merge primes and prime squares to find the n‑th element. */
    size_t i = 0;                 /* index for primes themselves   */
    size_t j = 0;                 /* index for prime squares       */
    idx_t sqrt_bound = floor_sqrt_u64(bound);
    size_t sq_limit = count_primes_leq(sqrt_bound);   /* primes whose square ≤ bound */

    seq_t val = 0;
    size_t seen = 1;              /* we already account for the value 1 */

    while (seen < n) {
        seq_t next_prime =
            (i < primes_cnt) ? (seq_t)primes[i] : (seq_t)UINT64_MAX;
        seq_t next_square =
            (j < sq_limit)
                ? (seq_t)((unsigned __int128)primes[j] * primes[j])
                : (seq_t)UINT64_MAX;

        if (next_prime <= next_square) {
            val = next_prime;
            i++;
        } else {
            val = next_square;
            j++;
        }
        seen++;
    }

    return val;
}
