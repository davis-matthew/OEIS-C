/*
 * A215007  a(n) = 7*a(n-1) - 14*a(n-2) + 7*a(n-3), a(0)=1, a(1)=3, a(2)=9.
 * https://oeis.org/A215007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 14*a(n-2) + 7*a(n-3) with a(0)=1, a(1)=3,
 *            a(2)=9
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_last = 2; /* last element computed */

    /* Lazy initialization of cache and base terms */
    if (!cache) {
        cache_cap = 8;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed – unlikely */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)9;
    }

    /* If the value is already cached, return it */
    if (n <= cache_last) {
        return cache[n];
    }

    /* Grow the cache if necessary */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap : 8;
        while (new_cap <= (size_t)n) new_cap <<= 1;  /* double until big enough */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failed – unlikely */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to n using the recurrence */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        cache[i] = (seq_t)7 * cache[i - 1]
                 - (seq_t)14 * cache[i - 2]
                 + (seq_t)7 * cache[i - 3];
    }
    cache_last = n;
    return cache[n];
}
