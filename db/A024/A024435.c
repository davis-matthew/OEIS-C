/*
 * A024435  Expansion of 1/((1-x)(1-6x)(1-10x)(1-12x)).
 * https://oeis.org/A024435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 29*a(n-1) - 280*a(n-2) + 972*a(n-3) - 720*a(n-4); *
 *            a(0)=1, a(1)=29, a(2)=561, a(3)=9121
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;          /* number of valid entries cache[0..cache_len-1] */

    if (cache_len <= (size_t)n) {         /* need to grow the cache up to n */
        size_t new_len = (size_t)n + 1;    /* elements 0..n inclusive */

#ifdef __cplusplus
        /* Allocate a new array with operator new[] */
        seq_t *new_cache = new seq_t[new_len];
        if (!new_cache) abort();

        /* Copy existing entries, if any, into the new array and free old */
        if (cache) {
            for (size_t i = 0; i < cache_len; ++i) {
                new_cache[i] = cache[i];
            }
            delete[] cache;
        }
        cache = new_cache;
#else
        /* In C, realloc performs the copy automatically */
        seq_t *tmp = realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
#endif

        /* Compute the new terms */
        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            if (i == 0)
                cache[i] = 1;
            else if (i == 1)
                cache[i] = 29;
            else if (i == 2)
                cache[i] = 561;
            else if (i == 3)
                cache[i] = 9121;
            else
                cache[i] = 29 * cache[i-1] - 280 * cache[i-2]
                           + 972 * cache[i-3] - 720 * cache[i-4];
        }
        cache_len = new_len;
    }

    return cache[n];
}
