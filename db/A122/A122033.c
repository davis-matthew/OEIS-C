/*
 * A122033  a(n) = 2*a(n-1) - 2*(n-2)*a(n-2), with a(0)=1, a(1)=2.
 * https://oeis.org/A122033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, less
 * Formula:   a(n) = 2*a(n-1) - 2*(n-2)*a(n-2), with a(0)=1, a(1)=2
 * Range:     i8 n <= 6, i16 n <= 11, i32 n <= 17, i64 n <= 29, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Cache storage – one implementation for C, one for C++ */
#ifdef __cplusplus
static std::vector<seq_t> cache;
#else
static seq_t *cache = NULL;
static size_t cache_size = 0;
#endif

/* Extend the cache up to (and including) index n */
static void ensure_cache(idx_t n)
{
#ifdef __cplusplus
    if (cache.size() <= n) {
        size_t old_size = cache.size();
        cache.resize(n + 1);

        /* Initialise base cases if the cache was empty */
        if (old_size == 0) {
            cache[0] = (seq_t)1;
            if (n >= 1) cache[1] = (seq_t)2;
        } else if (old_size == 1 && n >= 1) {
            cache[1] = (seq_t)2;
        }

        /* Start computing from the first unset index */
        idx_t start = old_size;
        if (start < 2) start = 2;
        for (idx_t i = start; i <= n; ++i) {
            cache[i] = (seq_t)2 * cache[i-1]
                     - (seq_t)2 * (seq_t)(i-2) * cache[i-2];
        }
    }
#else
    if (cache_size <= n) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();          /* allocation failure – should not happen */
        cache = new_cache;

        size_t old_size = cache_size;
        cache_size = new_size;

        if (old_size == 0) {
            cache[0] = (seq_t)1;
            if (new_size > 1) cache[1] = (seq_t)2;
            for (idx_t i = 2; i <= n; ++i) {
                cache[i] = (seq_t)2 * cache[i-1]
                         - (seq_t)2 * (seq_t)(i-2) * cache[i-2];
            }
        } else {
            idx_t start = old_size;
            if (start == 1 && new_size > 1) {
                cache[1] = (seq_t)2;
                start = 2;
            }
            for (idx_t i = start; i <= n; ++i) {
                cache[i] = (seq_t)2 * cache[i-1]
                         - (seq_t)2 * (seq_t)(i-2) * cache[i-2];
            }
        }
    }
#endif
}

/* Compute a(n) – memoised recurrence */
seq_t a(idx_t n)
{
    ensure_cache(n);
#ifdef __cplusplus
    return cache[n];
#else
    return cache[n];
#endif
}
