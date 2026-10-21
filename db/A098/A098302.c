/*
 * A098302  Member r=17 of the family of Chebyshev sequences S_r(n) defined in
 *          A092184.
 * https://oeis.org/A098302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-1) - a(n-2) + 2, a(0)=0, a(1)=1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of cached elements (last index + 1) */

seq_t a(idx_t n)
{
    /* Ensure the cache holds values up to index n */
    if (n + 1 > cache_len) {
        size_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) abort();      /* cannot recover */
        cache = new_cache;

        /* Compute all new entries */
        for (size_t i = cache_len; i < new_len; ++i) {
            if (i == 0)
                cache[i] = 0;
            else if (i == 1)
                cache[i] = 1;
            else
                cache[i] = 15 * cache[i - 1] - cache[i - 2] + 2;
        }
        cache_len = new_len;
    }
    return cache[n];
}
