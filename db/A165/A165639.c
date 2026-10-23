/*
 * A165639  a(n)=(7/4)*(1+3*(-7)^(n-1)).
 * https://oeis.org/A165639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 11, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 1, a(n) = -7 * a(n-1) + 14  (n >= 1)
 *
 * This implementation uses an incremental memoisation cache.  The
 * sequence grows exponentially, so for arbitrary-precision builds
 * the values are held in `seq_t`.  For n up to ~10000 the iterative
 * method runs well below the 2 s per call limit.
 */

#include <stdlib.h>   // malloc, realloc, NULL
#include <stddef.h>   // size_t

seq_t a(idx_t n)
{
    /* Static cache to store already computed values. */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;   /* allocated capacity of cache */
    static size_t cache_len  = 0;   /* number of computed entries */

    /* Initialise the cache on first call. */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) return (seq_t)0;   /* allocation failed – return 0 */
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Ensure the cache is large enough for index n. */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) return (seq_t)0;   /* realloc failed – return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing entries up to n. */
    if (cache_len <= (size_t)n) {
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            /* Use cast to seq_t for literals to avoid integer promotion issues. */
            cache[i] = (seq_t)(-7) * cache[i - 1] + (seq_t)14;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
