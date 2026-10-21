/*
 * A070866  Smallest prime such that the difference of successive terms is
 *          nondecreasing.
 * https://oeis.org/A070866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2, a(2)=3; a(n)=A007918(2*a(n-1)-a(n-2)) for n≥3
 * Range:     u8 n <= 20, u16 n <= 156, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A007918 is the smallest prime ≥ n. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrime.h"

/* find the least prime ≥ val (val is 0 or larger) */
static idx_t next_prime(idx_t val)
{
    if (val <= 2)
        return 2;          /* 2 is the first prime */
    /* make val odd if necessary */
    if ((val & 1) == 0)
        ++val;
    while (!isPrime(val))
        val += 2;          /* skip even numbers */
    return val;
}

/* cached sequence values */
static seq_t *cache = NULL;
static idx_t cache_len  = 0;   /* number of cached terms */
static idx_t cache_cap  = 0;   /* allocation capacity (in seq_t elements) */

/* grow the cache to at least 'needed' elements */
static void grow_cache(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 2;
    if (new_cap < needed)
        new_cap = needed;
    cache = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for n < 1, return 0 arbitrarily */
        return (seq_t)0;

    /* Initialize cache with first two terms if empty */
    if (cache_len == 0) {
        grow_cache(2);
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)3;
        cache_len = 2;
    }

    if (n <= cache_len)        /* requested term already cached */
        return cache[n - 1];

    /* Use idx_t for intermediate arithmetic to avoid overflow */
    idx_t prevprev = (idx_t)cache[cache_len - 2];
    idx_t prev     = (idx_t)cache[cache_len - 1];

    for (idx_t i = cache_len + 1; i <= n; ++i) {
        idx_t val = 2 * prev - prevprev;          /* the candidate argument */
        idx_t next = next_prime(val);             /* next prime ≥ val */

        grow_cache(i);
        cache[i - 1] = (seq_t)next;

        prevprev = prev;
        prev     = next;
        ++cache_len;
    }

    return cache[n - 1];
}
