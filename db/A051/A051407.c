/*
 * A051407  a(n) = 3^n*(3^(n+1)+1)/2.
 * https://oeis.org/A051407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n * (3^(n+1) + 1) / 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 2, a(1) = 15, a(n) = 12*a(n-1) - 27*a(n-2) for n > 1 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements currently computed */
    static idx_t cache_cap  = 0;   /* allocated capacity */

    /* Ensure the cache is large enough to hold index n */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 4;
        if (new_cap <= n) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();  /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)2;
        } else if (i == 1) {
            cache[i] = (seq_t)15;
        } else {
            cache[i] = cache[i-1] * (seq_t)12 - cache[i-2] * (seq_t)27;
        }
    }

    /* Update cached size */
    cache_size = n + 1;

    return cache[n];
}
