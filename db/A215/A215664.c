/*
 * A215664  a(n) = 3*a(n-2) - a(n-3), with a(0)=3, a(1)=0, and a(2)=6.
 * https://oeis.org/A215664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*a(n-2) - a(n-3), a(0)=3, a(1)=0, a(2)=6
 * Range:     i8 n <= 7, i16 n <= 16, i32 n <= 34, i64 n <= 69, i128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* external linkage as required */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of valid entries in cache */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* initialise cache with the first three terms */
    if (cache == NULL) {
        cache_cap = 3;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* on OOM: return 0, unlikely in tests */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)6;
        cache_len = 3;
    }

    /* if we already have the value, return it immediately */
    if (n < (idx_t)cache_len) {
        return cache[(size_t)n];
    }

    /* grow the cache if necessary */
    if (n >= (idx_t)cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* OOM fallback */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)3 * cache[i-2] - cache[i-3];
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
