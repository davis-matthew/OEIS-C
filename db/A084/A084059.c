/*
 * A084059  a(n) = 4*a(n-1) + 2*a(n-2) for n>1, a(0)=1, a(1)=2.
 * https://oeis.org/A084059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 2*a(n-2), a(0)=1, a(1)=2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for repeated calls */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* number of terms currently valid (0..cache_len-1) */
    static idx_t cache_alloc = 0;     /* allocated size */

    /* Ensure cache can hold up to n */
    if (cache_len <= n) {
        idx_t needed = n + 1;
        if (needed > cache_alloc) {
            /* grow cache: double until large enough */
            idx_t new_alloc = cache_alloc ? cache_alloc : 2;
            while (new_alloc < needed)
                new_alloc <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
            if (!tmp) {
                /* allocation failed: abort or return 0 */
                abort();
            }
            cache = tmp;
            cache_alloc = new_alloc;
        }
        /* initialise missing terms */
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
            cache_len = 1;
        }
        if (cache_len == 1 && n >= 1) {
            cache[1] = (seq_t)2;
            cache_len = 2;
        }
        /* compute terms up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t term = (seq_t)4 * cache[i - 1] + (seq_t)2 * cache[i - 2];
            cache[i] = term;
        }
        cache_len = n + 1;
    }
    return cache[n];
}
