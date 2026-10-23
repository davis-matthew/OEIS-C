/*
 * A028065  Expansion of 1/((1-3x)(1-5x)(1-8x)(1-10x)).
 * https://oeis.org/A028065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 26*a(n-1) - 239*a(n-2) + 910*a(n-3) - 1200*a(n-4) with
 *            initial values a(0)=1, a(1)=26, a(2)=437, a(3)=6058
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Small initial values */
    static const seq_t init_vals[4] = {1, 26, 437, 6058};

    /* Cache for computed terms */
    static seq_t *cache     = NULL;
    static idx_t  cache_cap = 0;
    static idx_t  last_computed = 3; /* index of last computed term */

    /* Return directly if n is one of the initial terms */
    if (n < 4) {
        return init_vals[n];
    }

    /* Allocate and initialise cache on first use */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort(); /* allocation failure */

        cache[0] = init_vals[0];
        cache[1] = init_vals[1];
        cache[2] = init_vals[2];
        cache[3] = init_vals[3];
    }

    /* Grow cache if needed */
    if (cache_cap <= n) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 2 * cache_cap) new_cap = 2 * cache_cap;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to n if not already done */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i] =
            cache[i-1] * (seq_t)26
            - cache[i-2] * (seq_t)239
            + cache[i-3] * (seq_t)910
            - cache[i-4] * (seq_t)1200;
    }
    last_computed = n;

    return cache[n];
}
