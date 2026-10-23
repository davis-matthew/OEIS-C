/*
 * A025977  Expansion of 1/((1-2x)(1-4x)(1-8x)(1-10x)).
 * https://oeis.org/A025977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*a(n-1) - 196*a(n-2) + 624*a(n-3) - 640*a(n-4) with
 *            a(0)=1, a(1)=24, a(2)=380, a(3)=5040
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

/* Cached values: cache holds all computed terms up to a certain index.
   last_computed is (idx_t)-1 when the cache has not yet been initialized. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t last_computed = (idx_t)-1;

#ifdef __cplusplus
static std::vector<seq_t> vecCache;
#endif

seq_t a(idx_t n)
{
    /* Work out how many slots we need.  The sequence starts at index 0,
       so we need at least 4 slots to store the base values. */
    idx_t needed = (n < 4) ? 4 : n + 1;

#ifdef __cplusplus
    /* --------------------------------------------------------------------
       C++ implementation: use a fixed‑size vector for the cache.
       -------------------------------------------------------------------- */
    if (vecCache.size() < needed) {
        vecCache.resize(needed);              /* default‑construct new elements */
    }

    /* Initialize the first four terms once, when the cache is empty. */
    if (last_computed == (idx_t)-1) {
        vecCache[0] = (seq_t)1;
        vecCache[1] = (seq_t)24;
        vecCache[2] = (seq_t)380;
        vecCache[3] = (seq_t)5040;
        last_computed = 3;
    }

    /* Compute any missing terms up to n. */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            vecCache[i] =
                24 * vecCache[i - 1]
              - 196 * vecCache[i - 2]
              + 624 * vecCache[i - 3]
              - 640 * vecCache[i - 4];
        }
        last_computed = n;
    }

    return vecCache[n];
#else   /* --------------------------------------------------------------------
           C implementation: use a dynamically allocated array.
           -------------------------------------------------------------------- */
    /* Allocate or grow the array if necessary. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * needed);
        if (cache == NULL) abort();
        cache_len = needed;
    } else if (needed > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * needed);
        if (tmp == NULL) abort();
        cache = tmp;
        cache_len = needed;
    }

    /* Initialise base cases once. */
    if (last_computed == (idx_t)-1) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)24;
        cache[2] = (seq_t)380;
        cache[3] = (seq_t)5040;
        last_computed = 3;
    }

    /* Compute missing terms up to n. */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            cache[i] =
                24 * cache[i - 1]
              - 196 * cache[i - 2]
              + 624 * cache[i - 3]
              - 640 * cache[i - 4];
        }
        last_computed = n;
    }

    return cache[n];
#endif
}
