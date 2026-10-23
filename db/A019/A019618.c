/*
 * A019618  Expansion of 1/((1-4*x)*(1-7*x)*(1-10*x)).
 * https://oeis.org/A019618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=21, a(2)=303; for n>2,
 *            a(n)=21*a(n-1)-138*a(n-2)+280*a(n-3)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t   cache_cap = 0;   /* allocated capacity (in elements) */
    static idx_t   cache_len = 0;   /* number of initialized elements */

    /* Grow the cache if we need to store up to n */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? (cache_cap << 1) : 4;
        while (new_cap < n + 1)
            new_cap <<= 1;

        seq_t *tmp;
        if (cache == NULL) {
            tmp = (seq_t *)malloc((size_t)new_cap * sizeof(seq_t));
        } else {
            tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        }
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)1;
        } else if (i == 1) {
            val = (seq_t)21;
        } else if (i == 2) {
            val = (seq_t)303;
        } else {
            val = ( (seq_t)21 * cache[i-1] )
                - ( (seq_t)138 * cache[i-2] )
                + ( (seq_t)280 * cache[i-3] );
        }
        cache[i] = val;
    }
    cache_len = n + 1;

    return cache[n];
}
