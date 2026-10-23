/*
 * A132402  Binomial transform of A004524 starting at 1.
 * https://oeis.org/A132402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 14*a(n-2) + 16*a(n-3) - 8*a(n-4) with a(0)=1,
 *            a(1)=3, a(2)=7, a(3)=15
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;   /* array holding values up to cache_len-1 */
    static idx_t  cache_len = 0;  /* number of elements currently stored */
    static size_t cache_cap = 0;  /* allocated capacity of cache */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;          /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)15;
        cache_len = 4;
    }

    /* if requested n is already computed, return it */
    if (n < cache_len)
        return cache[n];

    /* extend cache if necessary */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap;
        while ((idx_t)new_cap <= n)
            new_cap <<= 1;                     /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return (seq_t)0;           /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = 6 * cache[i-1] - 14 * cache[i-2] + 16 * cache[i-3] - 8 * cache[i-4];
    }
    cache_len = n + 1;

    return cache[n];
}
