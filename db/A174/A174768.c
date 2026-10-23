/*
 * A174768  y-values in the solution to x^2 - 26*y^2 = 1.
 * https://oeis.org/A174768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(2)=10, for n>=3: a(n)=102*a(n-1)-a(n-2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;    /* number of values stored (>=0) */
    static idx_t cache_capacity = 0;

    /* initialize cache for values 1 and 2 */
    if (cache == NULL) {
        cache_capacity = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_capacity);
        if (cache == NULL) return (seq_t)0; /* out of memory, return 0 */
        cache[0] = (seq_t)0;   /* a(1) */
        cache[1] = (seq_t)10;  /* a(2) */
        cache_size = 2;
    }

    /* if requested n is already in cache, return it */
    if (n <= cache_size) {
        return cache[n - 1];
    }

    /* extend cache capacity if needed */
    if (n > cache_capacity) {
        idx_t new_cap = cache_capacity;
        while (n > new_cap) new_cap *= 2;
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        if (new_cache == NULL) return (seq_t)0; /* out of memory, return 0 */
        for (idx_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_capacity = new_cap;
    }

    /* compute new values up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t prev = cache[i - 2];          /* a(i-1) */
        seq_t prevprev = cache[i - 3];      /* a(i-2) */
        seq_t next = (seq_t)102 * prev - prevprev;
        cache[i - 1] = next;
    }
    cache_size = n;

    return cache[n - 1];
}
