/*
 * A180167  a(0) = 1, a(1) = 7; a(n)= 6*a(n-1) + 6*a(n-2) for n>1.
 * https://oeis.org/A180167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + 6*a(n-2), a(0) = 1, a(1) = 7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache of computed values.  cache_len = number of values
       that have already been computed (>= 2 after first allocation). */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;   /* number of filled entries       */
    static size_t cache_cap = 0;   /* allocated capacity (>= len)   */

    /* Initialise the cache if this is the first call. */
    if (cache_cap == 0) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* OOM – unlikely */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache_len = 2;
    }

    /* If the requested index is already known, return it quickly. */
    if (n < cache_len)
        return cache[n];

    /* Grow the allocation so that it can hold indices up to n. */
    if ((idx_t)(cache_cap) <= n) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* OOM – unlikely */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to and including n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = (seq_t)6 * cache[i - 1] + (seq_t)6 * cache[i - 2];
    }

    /* Update the number of computed entries. */
    cache_len = n + 1;

    return cache[n];
}
