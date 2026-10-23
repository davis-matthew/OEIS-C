/*
 * A121449  Expansion of (1 - 3*x + 2*x^2)/(1 - 4*x + 3*x^2 + x^3).
 * https://oeis.org/A121449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=3, a(n+1)=4*a(n)-3*a(n-1)-a(n-2) for n>=2
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values;  cache[i] == a(i) */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* initialise cache with the first three terms */
    if (!cache) {
        cache_size = 3;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();                     /* allocation failure  */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
    }

    /* extend cache if the requested index is not yet stored */
    if ((size_t)n >= cache_size) {
        size_t old_size = cache_size;
        cache_size = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        if (!cache) abort();                     /* allocation failure  */

        /* compute missing terms using the recurrence */
        for (idx_t i = (idx_t)old_size; i <= n; ++i) {
            seq_t val = (seq_t)4 * cache[i - 1];   /* 4*a(i-1) */
            val -= (seq_t)3 * cache[i - 2];        /* -3*a(i-2) */
            val -= cache[i - 3];                  /* -a(i-3)   */
            cache[i] = val;
        }
    }

    return cache[(size_t)n];
}
