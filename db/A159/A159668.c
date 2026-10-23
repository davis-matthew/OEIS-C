/*
 * A159668  Expansion of (1 - x)/(1 - 28*x + x^2).
 * https://oeis.org/A159668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 28*a(n-1) - a(n-2), a(0) = 1, a(1) = 27
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements currently stored */
    static size_t cache_cap = 0;   /* current capacity of the cache array */

    /* Ensure cache has at least n+1 entries */
    if (cache_len <= n) {
        /* Determine new capacity */
        size_t new_cap = cache_cap;
        if (new_cap == 0)
            new_cap = 16;
        while (new_cap <= n)
            new_cap <<= 1;           /* double until we can hold n */

        if (new_cap != cache_cap) {
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* Initialise missing values */
        for (size_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)27;
            } else {
                seq_t term = cache[i - 1];
                term = term * (seq_t)28;          /* multiply by 28 */
                term = term - cache[i - 2];       /* subtract previous term */
                cache[i] = term;
            }
        }
        cache_len = n + 1; /* Update length after filling */
    }

    return cache[(size_t)n];
}
