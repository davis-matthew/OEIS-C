/*
 * A214041  a(n) = a(n-1) + floor((a(n-2) + a(n-3))/2), with a(n)=1 for n < 3.
 * https://oeis.org/A214041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + floor((a(n-2)+a(n-3))/2), a(0)=a(1)=a(2)=1
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 53, u64 n <= 105,
 *            u128 n <= 208, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if defined(__cplusplus)
#include <algorithm>
#endif
#include <stdlib.h>

static seq_t *cache = NULL;          /* cache of sequence values */
static size_t cache_cap = 0;         /* current capacity of cache */
static size_t cache_last = (size_t)-1; /* index of last computed value */

/* Grow the cache to hold indices up to 'n' inclusive.  */
static void ensure_capacity(idx_t n)
{
    /* number of entries we need */
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap) return;

    /* new capacity: next power of two ≥ needed */
    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap < needed) new_cap <<= 1;

#if defined(__cplusplus)
    /* C++: allocate and copy */
    seq_t *new_cache = new seq_t[new_cap];
    if (cache) {
        /* copy old data */
        std::copy(cache, cache + cache_cap, new_cache);
        delete[] cache;
    }
#else
    /* C: realloc raw memory */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache && new_cap > 0) abort(); /* allocation failed */
#endif

    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the n-th term of sequence A214041.  */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n < 3) return (seq_t)1;

    /* First call: allocate the cache and set the initial terms */
    if (!cache) {
        ensure_capacity(n);
        cache[0] = cache[1] = cache[2] = (seq_t)1;
        cache_last = 2;
    }

    /* Ensure the cache covers the requested index */
    ensure_capacity(n);

    /* If already computed, return it */
    if ((size_t)n <= cache_last)
        return cache[(size_t)n];

    /* Compute all missing terms up to n */
    for (size_t i = cache_last + 1; i <= (size_t)n; ++i) {
        seq_t sum = cache[i - 2] + cache[i - 3];
        seq_t half = sum / (seq_t)2;      /* floor division by 2 */
        cache[i] = cache[i - 1] + half;
    }

    cache_last = (size_t)n;
    return cache[(size_t)n];
}
