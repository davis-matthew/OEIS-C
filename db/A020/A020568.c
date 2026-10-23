/*
 * A020568  G.f.: 1/((1-5x)(1-10x)(1-12x)).
 * https://oeis.org/A020568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27*a(n-1) - 230*a(n-2) + 600*a(n-3) Initial values:
 *            a(0)=1, a(1)=27, a(2)=499
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Memoized array of previously computed terms */
static seq_t *cache = NULL;     /* pointer to array */
static size_t cache_len = 0;    /* number of valid entries in cache */
static size_t cache_cap = 0;    /* allocated capacity of the array */

/* Ensure that cache contains values up to index n */
static void ensure_cache(idx_t n)
{
    /* Convert the index to a size_t; sequence length fits in size_t for
       all practical n under the time limit (e.g. n up to 10000). */
    size_t target = (size_t)n;

    /* Allocate initial cache if necessary */
    if (cache == NULL) {
        /* Start with a small capacity, grow as needed */
        cache_cap = 8;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return; /* allocation failure – caller will get 0 */
    }

    /* Fill the base values if not already present */
    if (cache_len < 3) {
        if (cache_cap < 3) {
            cache_cap = 3;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)27;
        cache[2] = (seq_t)499;
        cache_len = 3;
    }

    /* Extend the cache up to the requested index */
    if (target >= cache_cap) {
        /* Double capacity until it covers 'target' */
        while (cache_cap <= target) {
            cache_cap *= 2;
        }
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Compute missing terms */
    for (size_t i = cache_len; i <= target; ++i) {
        seq_t val = cache[i-1] * (seq_t)27      /* 27*a(i-1)   */
                   - cache[i-2] * (seq_t)230    /* 230*a(i-2)  */
                   + cache[i-3] * (seq_t)600;   /* 600*a(i-3)  */
        cache[i] = val;
    }
    cache_len = target + 1;
}

/* Public API: compute a(n) */
seq_t a(idx_t n)
{
    ensure_cache(n);
    if (n < cache_len) {
        return cache[(size_t)n];
    }
    /* In case of an allocation failure in ensure_cache, return 0 */
    return (seq_t)0;
}
