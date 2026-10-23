/*
 * A116796  Number of permutations of length n which avoid the patterns 2314,
 *          3241, 4132.
 * https://oeis.org/A116796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 17*a(n-2) + 17*a(n-3) - 7*a(n-4) + a(n-5) (for
 *            n > 5) with a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=72
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* cache the computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with the first five values */
    if (cache_size == 0) {
        /* allocate space for five terms */
        #ifdef __cplusplus
            cache = new seq_t[5];
        #else
            cache = (seq_t*)malloc(5 * sizeof(seq_t));
        #endif
        if (!cache) return 0;   /* allocation failure */

        cache[0] = 1;  /* a(1) */
        cache[1] = 2;  /* a(2) */
        cache[2] = 6;  /* a(3) */
        cache[3] = 21; /* a(4) */
        cache[4] = 72; /* a(5) */
        cache_size = 5;
    }

    /* if the requested index is already cached, return it */
    if (n <= cache_size)
        return cache[n-1];

    /* enlarge the cache to hold up to n terms */
    #ifdef __cplusplus
        /* allocate new array and copy old terms */
        seq_t *new_cache = new seq_t[n];
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        delete [] cache;
    #else
        seq_t *new_cache = (seq_t*)realloc(cache, n * sizeof(seq_t));
    #endif
    if (!new_cache) return 0;   /* allocation failure */
    cache = new_cache;

    /* compute the missing terms using the recurrence */
    for (idx_t i = cache_size; i < n; ++i) {
        /* recurrence: a[i] = 7*a[i-1] - 17*a[i-2] + 17*a[i-3] - 7*a[i-4] + a[i-5] */
        seq_t t0 = cache[i-5];
        seq_t t1 = cache[i-4];
        seq_t t2 = cache[i-3];
        seq_t t3 = cache[i-2];
        seq_t t4 = cache[i-1];
        cache[i] = 7 * t4 - 17 * t3 + 17 * t2 - 7 * t1 + t0;
    }
    cache_size = n;

    return cache[n-1];
}
