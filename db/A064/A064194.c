/*
 * A064194  a(2n) = 3*a(n), a(2n+1) = 2*a(n+1)+a(n), with a(1) = 1.
 * https://oeis.org/A064194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(2n) = 3*a(n), a(2n+1) = 2*a(n+1)+a(n), a(1) = 1
 * Range:     u8 n <= 32, u16 n <= 1030, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: iterative construction with a growing static cache. */
/* Works for any seq_t (uint8_t … unsigned __int128 … arbitrary‑precision) */
/* and any idx_t (uint64_t).  No external helper functions required. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache       = NULL;  /* dynamic cache; index 0 unused */
static idx_t  cache_size  = 0;     /* allocated size (elements) */
static idx_t  max_computed = 0;    /* highest n for which a(n) is known */

/* Ensure the cache can hold at least n+1 elements (index 0…n). */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;

    /* Grow: at least double, but also enough for n */
    idx_t new_size = cache_size > 0 ? cache_size : 16;
    while (new_size <= n)
        new_size <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
    if (!new_cache) {
        /* Out of memory – abort */
        abort();
    }
    cache = new_cache;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* offset is 1; value for 0 is irrelevant */

    ensure_cache(n);

    /* Initialise base case if not yet done */
    if (max_computed == 0) {
        cache[1] = (seq_t)1;
        max_computed = 1;
    }

    /* Compute all missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if ((i & 1) == 0) {                 /* i = 2*k */
            cache[i] = (seq_t)3 * cache[i >> 1];
        } else {                            /* i = 2*k + 1 */
            cache[i] = (seq_t)2 * cache[(i >> 1) + 1] + cache[i >> 1];
        }
    }

    max_computed = n;
    return cache[n];
}
