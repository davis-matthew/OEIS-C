/*
 * A016064  Smallest side lengths of almost-equilateral Heronian triangles
 *          (sides are consecutive positive integers, area is a nonnegative
 *          integer).
 * https://oeis.org/A016064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - a(n-2) + 2 (n ≥ 2), a(0) = 1, a(1) = 3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#if defined(__cplusplus)
    #include <cstddef>
#endif

/* static cache for computed terms */
static seq_t *cache = NULL;
static size_t  cache_len = 0;   /* number of valid entries in cache */
static size_t  cache_cap = 0;   /* allocated capacity of cache */

/* expand the cache to the requested capacity */
static void
cache_grow(size_t new_cap)
{
    seq_t *new_cache;
#if defined(__cplusplus)
    new_cache = new seq_t[new_cap];
#else
    new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
    for (size_t i = 0; i < cache_len; ++i) {
        new_cache[i] = cache[i];
    }
#if defined(__cplusplus)
    delete[] cache;
#else
    free(cache);
#endif
    cache        = new_cache;
    cache_cap    = new_cap;
}

seq_t a(idx_t n)
{
    /* use size_t for array indexing; n is small enough for this conversion */
    size_t idx = (size_t)n;
    size_t need = idx + 1;
    if (need < 2) need = 2;                       /* ensure at least 2 slots */

    /* allocate or resize cache */
    if (cache == NULL) {
#if defined(__cplusplus)
        cache = new seq_t[need];
#else
        cache = (seq_t*)malloc(need * sizeof(seq_t));
#endif
        cache_cap = need;
        cache_len = 0;
    } else if (need > cache_cap) {
        cache_grow(need);
    }

    /* initialise first two terms if not already present */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }
    if (cache_len == 1 && idx >= 1) {
        cache[1] = (seq_t)3;
        cache_len = 2;
    }

    /* compute missing terms up to idx */
    for (size_t i = cache_len; i <= idx; ++i) {
        seq_t val = (seq_t)4 * cache[i-1] - cache[i-2] + (seq_t)2;
        cache[i] = val;
    }
    cache_len = idx + 1;   /* all entries up to idx are valid */

    return cache[idx];
}
