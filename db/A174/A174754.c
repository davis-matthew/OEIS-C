/*
 * A174754  x-values in the solution to x^2-46*y^2=1.
 * https://oeis.org/A174754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 48670*a(n-1)-a(n-2) with a(1)=1, a(2)=24335
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* Ensure the cache can hold at least 'n' entries */
    if (cache == NULL || cache_cap < n) {
        size_t new_cap = (cache_cap == 0) ? 2 : cache_cap;
        while (new_cap < n)
            new_cap <<= 1;                       /* grow by factor 2 */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                      /* unrecoverable allocation failure */
        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* Initialise first two terms once */
    if (cache_len < 2) {
        cache[0] = (seq_t)1;                   /* a(1) */
        cache[1] = (seq_t)24335;                /* a(2) */
        cache_len = 2;
    }

    /* Compute missing terms up to n */
    if (n > cache_len) {
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            /* a(i) = 48670*a(i-1) - a(i-2) */
            cache[i-1] = ((seq_t)48670) * cache[i-2] - cache[i-3];
        }
        cache_len = n;
    }

    return cache[n-1];
}
