/*
 * A052973  Expansion of ( 1-x ) / ( 1-x-3*x^2+x^3 ).
 * https://oeis.org/A052973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=0, a(2)=3 a(n) = 3*a(n-2) + a(n-1) - a(n-3) for n ≥
 *            3 (equivalent to a(n+3)=3*a(n+1)+a(n+2)-a(n))
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 58, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;   /* cache[0..cache_len-1] */
    static idx_t cache_len = 0;   /* number of cached entries */

    /* initialize cache with first three terms if empty */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;          /* fallback on allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)3;
    }

    /* If requested n is already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* Extend cache to accommodate n */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0; /* fallback on allocation failure */
        cache = tmp;

        /* Compute new terms from current cache_len up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* a(i) = 3*a(i-2) + a(i-1) - a(i-3)  (i ≥ 3) */
            seq_t val = (seq_t)3 * cache[i - 2] + cache[i - 1] - cache[i - 3];
            cache[i] = val;
        }
        cache_len = new_len;
    }

    return cache[n];
}
