/*
 * A024025  a(n) = 3^n - n^2.
 * https://oeis.org/A024025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - n^2 Recurrence for n > 3: a(n) = 6*a(n-1) -
 *            12*a(n-2) + 10*a(n-3) - 3*a(n-4) Initial values: a(0)=1, a(1)=2,
 *            a(2)=5, a(3)=18 This implementation memoises results in a static
 *            cache so calls in any order remain efficient
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i) for i < cache_len */
    static seq_t *cache   = NULL;
    static idx_t  cache_len = 0;

    /* initialise cache with the first four terms if empty */
    if (cache_len == 0) {
        cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failure fallback */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)18;
    }

    /* Return cached value if already available */
    if (n < cache_len)
        return cache[n];

    /* grow cache up to n inclusive */
    size_t new_len = (size_t)n + 1;
    if (new_len > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure fallback */
        cache = tmp;
        /* compute missing terms using the recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = (seq_t)(6 * cache[i - 1]
                               - 12 * cache[i - 2]
                               + 10 * cache[i - 3]
                               - 3 * cache[i - 4]);
        }
        cache_len = new_len;
    }

    return cache[n];
}
