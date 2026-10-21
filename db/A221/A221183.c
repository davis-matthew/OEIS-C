/*
 * A221183  a(0)=0, a(1)=1; thereafter a(n) = gpf(2*a(n-1)+a(n-2)), where gpf
 *          = "greatest prime factor" (A006530).
 * https://oeis.org/A221183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0, a(1) = 1; a(n) = gpf(2*a(n-1)+a(n-2)) */
#include <stdlib.h>
#include "../utils/gpf.h"

static seq_t *cache = NULL;      /* memoisation table                     */
static size_t cache_len = 0;     /* number of cached elements (indices 0..len-1) */

/* Grow the cache so it can hold indices up to max_n (inclusive).          */
static void ensure_cache(idx_t max_n)
{
    if (cache_len > (size_t)max_n) return;
    size_t new_len = (size_t)(max_n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) exit(1);                /* out‑of‑memory                           */
    cache = tmp;
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    /* base values */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* initialise memoisation table on first call */
    if (!cache)
    {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* return cached value if available */
    if ((size_t)n < cache_len) return cache[n];

    /* remember old length before reallocation so we know where to start filling */
    size_t old_len = cache_len;
    ensure_cache(n);

    /* compute all missing terms up to n */
    for (size_t i = old_len; i <= (size_t)n; ++i)
    {
        uint64_t prev1 = (uint64_t)cache[i-1];
        uint64_t prev2 = (uint64_t)cache[i-2];
        uint64_t sum   = (uint64_t)2 * prev1 + prev2;   /* 2*a(i-1)+a(i-2)   */
        cache[i] = (seq_t)gpf(sum);                     /* greatest prime factor */
    }
    return cache[n];
}
