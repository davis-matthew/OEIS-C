/*
 * A041715  Denominators of continued fraction convergents to sqrt(377).
 * https://oeis.org/A041715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 466*a(n-4) - a(n-8) for n > 7 Initial values (for n = 0 …
 *            7): 1, 2, 5, 12, 461, 934, 2329, 5592
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static const seq_t init_vals[8] = {1, 2, 5, 12, 461, 934, 2329, 5592};

seq_t a(idx_t n)
{
    /* Base cases */
    if (n < 8)
        return init_vals[n];

    /* Allocate or grow the cache as needed */
    if (cache_len <= (size_t)n) {
        size_t new_len = cache_len ? cache_len * 2 : (size_t)n + 1;
        if (new_len < (size_t)n + 1)
            new_len = (size_t)n + 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)
            return 0;  /* unlikely, but avoid crash */

        cache = tmp;

        /* Initialise first 8 entries if the cache was empty */
        if (cache_len == 0) {
            for (size_t i = 0; i < 8; ++i)
                cache[i] = init_vals[i];
            cache_len = 8;
        }

        /* Compute missing terms up to n */
        for (idx_t i = cache_len; i <= n; ++i)
            cache[i] = (seq_t)466 * cache[i - 4] - cache[i - 8];

        cache_len = (size_t)n + 1;
    }

    return cache[n];
}
