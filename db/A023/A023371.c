/*
 * A023371  a(n+1) = a(n) converted to base 8 from base 3 (written in base
 *          10).
 * https://oeis.org/A023371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n+1) = a(n) converted to base 8 from base 3 (written in base
 *            10)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   base3_to_base8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/base3_to_base8.h"

seq_t a(idx_t n)
{
    /* The cache holds a(1) .. a(cache_size).  cache_size == 0 means no cache yet. */
    static seq_t *cache        = NULL;
    static idx_t cache_size    = 0;
    static idx_t cache_cap     = 0;

    if (n == 0) {               /* offset is 1; undefined for n=0, but return 0 */
        return 0;
    }

    /* Ensure the first value is in the cache. */
    if (cache_size == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) exit(1);           /* out of memory */
        cache[0] = 3;                  /* a(1) */
        cache_size = 1;
        cache_cap = 1;
    }

    /* Extend the cache if we need to compute further terms. */
    if (n > cache_size) {
        idx_t new_cap = cache_cap;
        while (new_cap < n)
            new_cap *= 2;             /* grow exponentially to reduce reallocs */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;

        /* Compute the missing values iteratively. */
        for (idx_t i = cache_size; i < n; ++i) {
            cache[i] = base3_to_base8(cache[i - 1]);
        }
        cache_size = n;
    }

    return cache[n - 1];
}
