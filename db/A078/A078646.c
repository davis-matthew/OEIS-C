/*
 * A078646  Number of representations of n as a sum of two primes that are
 *          congruent modulo 3.
 * https://oeis.org/A078646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Compute a(n) = number of unordered pairs of primes (p,q) with p+q=n
 * and p ≡ q (mod 3).  The function uses a sieve to pre‑compute primes
 * up to the requested n and then counts the valid pairs.  It stores
 * the results in a static cache so that subsequent calls are fast.
 * The algorithm runs in O(n·π(n)) time with n ≤ 10 000 easily under
 * the 2 s limit.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Integer square root for idx_t values (treats arguments as unsigned). */
static idx_t _isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi)
    {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if (mid <= n / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Cached data for the sieve. */
static idx_t       _sieve_limit   = 0;    /* largest n already processed */
static bool       *_is_prime      = NULL; /* prime flag array */
static uint64_t   *_primes        = NULL; /* list of primes up to _sieve_limit */
static size_t      _prime_cnt     = 0;    /* number of primes in _primes */
static seq_t      *_count_cache   = NULL; /* a(1) … a(_sieve_limit) */
static size_t      _cache_len     = 0;    /* length of _count_cache */

/* Rebuilds everything up to the given limit (inclusive). */
static void _rebuild(idx_t limit)
{
    /* Free old data. */
    free(_is_prime);
    free(_primes);
    free(_count_cache);

    /* Allocate prime flag array. */
    _is_prime = (bool *)calloc(limit + 1, sizeof(bool));
    if (_is_prime == NULL)
        return; /* allocation failure, nothing can be computed */
    _is_prime[0] = false;
    if (limit >= 1) _is_prime[1] = false;
    for (idx_t i = 2; i <= limit; ++i)
        _is_prime[i] = true;

    /* Sieve of Eratosthenes. */
    idx_t sq = _isqrt_u64(limit);
    for (idx_t i = 2; i <= sq; ++i)
    {
        if (_is_prime[i])
        {
            for (idx_t j = i * i; j <= limit; j += i)
                _is_prime[j] = false;
        }
    }

    /* Build prime list. */
    _prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (_is_prime[i]) ++_prime_cnt;
    _primes = (uint64_t *)malloc(_prime_cnt * sizeof(uint64_t));
    if (_primes == NULL)
    {
        _prime_cnt = 0;
        return; /* allocation failure */
    }
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (_is_prime[i]) _primes[pos++] = (uint64_t)i;

    /* Allocate cache for results. */
    _count_cache = (seq_t *)malloc(limit * sizeof(seq_t));
    if (_count_cache == NULL)
    {
        _cache_len = 0;
        return; /* allocation failure */
    }
    _cache_len = limit;

    /* Compute a(n) for all n up to limit. */
    for (idx_t m = 1; m <= limit; ++m)
    {
        seq_t cnt = 0;
        for (size_t pi = 0; pi < _prime_cnt; ++pi)
        {
            uint64_t p = _primes[pi];
            if ((idx_t)p > m / 2) break; /* p > q */
            uint64_t q = (uint64_t)m - p;
            if (q < p) continue;        /* enforce p <= q */
            if (q <= (uint64_t)limit && _is_prime[q] &&
                ((p % 3U) == (q % 3U)))
                ++cnt;
        }
        _count_cache[m - 1] = cnt;
    }

    _sieve_limit = limit;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (n > _sieve_limit) _rebuild(n);
    return _count_cache[n - 1];
}
