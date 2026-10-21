/*
 * A077234  Bisection (odd part) of Chebyshev sequence with Diophantine
 *          property.
 * https://oeis.org/A077234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0)=2, a(1)=9; a(n)=4*a(n-1)-a(n-2) for n≥2
 */

#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* return cached value if it exists */
    if (n < cache_len)
        return cache[n];

    /* initialise cache with the first two terms */
    if (cache_len < 2) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;              /* allocation failed */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)9;
        cache_len = 2;
    }

    /* expand cache up to index n (if necessary) */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;              /* allocation failed */

        cache = tmp;

        /* compute new terms using the recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = 4 * cache[i - 1] - cache[i - 2];
        }
        cache_len = new_len;
    }

    return cache[n];
}
