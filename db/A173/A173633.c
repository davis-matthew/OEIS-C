/*
 * A173633  a(n) = a(n-2) + a(n-3) - [a(n-3)/4] - [a(n-4)/2] - [a(n-5)/4].
 * https://oeis.org/A173633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            a(n-2)+a(n-3)-Floor[a(n-3)/4]-Floor[a(n-4)/2]-Floor[a(n-5)/4]
 *            Initial values: a(0)=1, a(1)=1, a(2)=1, a(3)=2, a(4)=2, a(5)=3
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t _init_vals[6] = {1, 1, 1, 2, 2, 3};

seq_t a(idx_t n)
{
    /* For n < 6 just return the hard‑coded values */
    if (n < 6)
        return _init_vals[n];

    /* Static cache for computed values */
    static seq_t *cache     = NULL;   /* array of cached terms */
    static idx_t  cache_len = 0;      /* number of terms stored */
    static size_t cache_cap = 0;      /* allocated capacity */

    /* Allocate and initialise the cache if necessary */
    if (cache_len == 0) {
        size_t cap = 6;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)
            exit(EXIT_FAILURE);   /* allocation failure */
        for (idx_t i = 0; i < 6; ++i)
            cache[i] = _init_vals[i];
        cache_len = 6;
        cache_cap = cap;
    }

    /* Grow the cache if we need more space */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < n + 1)
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i < 6)
            val = _init_vals[i];
        else
            val = cache[i - 2] + cache[i - 3]
                  - (cache[i - 3] / (seq_t)4)
                  - (cache[i - 4] / (seq_t)2)
                  - (cache[i - 5] / (seq_t)4);
        cache[i] = val;
    }
    cache_len = n + 1;

    return cache[n];
}
