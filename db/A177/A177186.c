/*
 * A177186  a(n+1) = a(n) + p, where p is the largest prime dividing a(n) but
 *          not a(n-1), or 1 if there is no such prime.
 * https://oeis.org/A177186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 227, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(1) = 1; for n > 1,
 *   a(n) = a(n-1) + p, where p is the largest prime divisor of a(n-1)
 *   that does not divide a(n-2). If no such prime exists, p = 1. 
 * Memoised implementation suitable for any order of calls. 
 */

#include <stdint.h>
#include <stdlib.h>

/* Forward declaration of helper to obtain distinct prime factors. */
static void factor_distinct(uint64_t n, uint64_t *pf, size_t *cnt);

/* Static cache for computed terms. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Ensure the cache contains at least `n` terms. */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n)
        return;

    /* Allocate new cache of the required size. */
    seq_t *new_cache = (seq_t *)malloc(n * sizeof(seq_t));
    if (!new_cache)
        return; /* allocation failed – caller will handle 0 result */

    /* Copy existing terms. */
    for (idx_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    /* Compute missing terms. */
    for (idx_t i = cache_len; i < n; ++i)
    {
        if (i == 0) /* a(1) */
        {
            new_cache[i] = (seq_t)1;
        }
        else if (i == 1) /* a(2) */
        {
            new_cache[i] = (seq_t)2;
        }
        else
        {
            /* Previous two terms.  Cast to uint64_t for factorisation. */
            uint64_t prevval      = (uint64_t)new_cache[i - 1];
            uint64_t prevprevval  = (uint64_t)new_cache[i - 2];

            /* Get distinct prime factors of the previous term. */
            uint64_t primes[64];
            size_t pf_cnt = 0;
            factor_distinct(prevval, primes, &pf_cnt);

            /* Find the largest prime that does not divide the element before
               previous.  Scan the list from largest to smallest. */
            uint64_t p = 1;
            for (size_t j = pf_cnt; j > 0; --j)
            {
                uint64_t prime = primes[j - 1];
                if (prevprevval % prime != 0)
                {
                    p = prime;
                    break;
                }
            }

            new_cache[i] = new_cache[i - 1] + (seq_t)p;
        }
    }

    /* Replace the old cache and free its memory. */
    free(cache);
    cache = new_cache;
    cache_len = n;
}

/* Obtain distinct prime divisors of n (ascending order).  Result array
 * must be able to hold at least 64 elements. */
static void factor_distinct(uint64_t n, uint64_t *pf, size_t *cnt)
{
    size_t c = 0;
    if (n % 2ULL == 0ULL)
    {
        pf[c++] = 2ULL;
        while (n % 2ULL == 0ULL)
            n /= 2ULL;
    }
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL)
    {
        if (n % d == 0ULL)
        {
            pf[c++] = d;
            while (n % d == 0ULL)
                n /= d;
        }
    }
    if (n > 1ULL)
        pf[c++] = n;
    *cnt = c;
}

/* Public sequence routine.  Returns the n‑th term (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n < 1 */

    ensure_cache(n);
    return cache[n - 1];
}
