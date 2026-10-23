/*
 * A136298  a(n) = 3*a(n-1) - 4*a(n-3), with a(0)=1, a(1)=2, a(2)=4, a(3)=9.
 * https://oeis.org/A136298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-3) Initial values: a(0)=1, a(1)=2,
 *            a(2)=4, a(3)=9
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 30, u64 n <= 61, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for realloc, abort */

/* static cache of already computed terms */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* first unused index */

/* compute a(n) for arbitrary non‑negative idx_t */
seq_t a(idx_t n)
{
    /* extend the cache if required */
    if (n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)
            abort();   /* allocation failed */
        cache = tmp;

        /* initialise the new entries */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0)       cache[i] = 1;
            else if (i == 1)  cache[i] = 2;
            else if (i == 2)  cache[i] = 4;
            else if (i == 3)  cache[i] = 9;
            else              cache[i] = 3 * cache[i - 1] - 4 * cache[i - 3];
        }
        cache_len = (idx_t)new_len;
    }
    return cache[n];
}
