/*
 * A070782  a(n) = Sum_{k=0..n} binomial(5*n,5*k).
 * https://oeis.org/A070782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 21*a(n-1) + 353*a(n-2) - 32*a(n-3) for n > 2 Initial
 *            values: a(0)=1, a(1)=2, a(2)=254
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* allocated size (number of elements)   */
    static idx_t last = 0;        /* highest index whose value is valid */

    /* ensure the cache has room for index n */
    if (cache_len <= n) {
        idx_t new_len = cache_len ? cache_len : 8;
        while (new_len <= n) new_len <<= 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache_len = new_len;
    }

    /* initialise the first three terms when required */
    if (n == 0) {
        cache[0] = 1;
        last = 0;
        return 1;
    }
    if (n == 1) {
        cache[1] = 2;
        last = 1;
        return 2;
    }
    if (n == 2) {
        cache[2] = 254;
        last = 2;
        return 254;
    }

    /* for n > 2, make sure terms up to n are computed */
    if (n > last) {
        /* ensure cache[0], cache[1], cache[2] are set */
        cache[0] = 1;
        cache[1] = 2;
        cache[2] = 254;
        /* compute new values up to n */
        for (idx_t i = last + 1; i <= n; ++i) {
            cache[i] = 21 * cache[i - 1] + 353 * cache[i - 2] - 32 * cache[i - 3];
        }
        last = n;
    }

    return cache[n];
}
