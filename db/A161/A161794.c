/*
 * A161794  Largest prime gap for primes less than (n+1)^2.
 * https://oeis.org/A161794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Largest prime gap for primes less than (n+1)^2.
   a(n) = max{ p_{k+1} - p_k | p_k < (n+1)^2 } */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/sieve_primes.h"   /* provides void sieve_primes(idx_t, idx_t**, size_t*) */

static idx_t *prime_cache = NULL;   /* primes already sieved up to cache_limit */
static size_t prime_cnt  = 0;
static idx_t cache_limit = 0;

/* enlarge the sieve cache to cover the requested limit */
static void ensure_limit(idx_t needed_limit)
{
    if (needed_limit <= cache_limit)
        return;                 /* nothing to do */

    /* discard old cache */
    free(prime_cache);

    /* build new sieve up to needed_limit */
    sieve_primes(needed_limit, &prime_cache, &prime_cnt);

    cache_limit = needed_limit;
}

seq_t a(idx_t n)
{
    if (n == 0)                /* offset 1 – not used by the problem statement */
        return 0;

    /* limit = (n+1)^2 - 1  ->  primes < (n+1)^2 */
    idx_t limit = ((idx_t)n + 1) * ((idx_t)n + 1) - 1;

    ensure_limit(limit);

    /* locate last prime <= limit (binary search) */
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = lo + ((hi - lo) >> 1);
        if (prime_cache[mid] <= limit)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo == 0)              /* no primes ≤ limit – impossible for the problem range */
        return 0;
    size_t idx = lo - 1;      /* index of the last prime ≤ limit */

    /* compute the largest gap among primes ≤ limit */
    seq_t max_gap = 0;
    for (size_t i = idx; i > 0; --i) {
        seq_t gap = (seq_t)(prime_cache[i] - prime_cache[i - 1]);
        if (gap > max_gap)
            max_gap = gap;
    }
    return max_gap;
}
