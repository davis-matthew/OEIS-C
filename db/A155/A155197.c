/*
 * A155197  a(n) = 8*a(n-1) + a(n-2) for n>2, with a(0)=1, a(1)=7, a(2)=56.
 * https://oeis.org/A155197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) + a(n-2) for n>2, a(0)=1, a(1)=7, a(2)=56
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache holds all computed values */
    static seq_t *cache   = NULL;
    static idx_t  cache_len = 0;   /* number of cached terms */

    /* First call: allocate cache for the first three terms */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)56;
    }

    /* Ensure we have space up to index n */
    if (n >= cache_len) {
        idx_t old_len = cache_len;
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache = tmp;
        cache_len = new_len;

        /* Compute any missing terms up to n */
        for (idx_t i = old_len; i <= n; ++i) {
            /* recurrence: a(i) = 8*a(i-1) + a(i-2) */
            cache[i] = cache[i-1] * (seq_t)8 + cache[i-2];
        }
        return cache[n];
    }

    /* All requested values are already cached */
    return cache[n];
}
