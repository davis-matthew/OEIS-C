/*
 * A122509  Expansion of x/(1 - 2*x^2 - 21*x^3).
 * https://oeis.org/A122509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) + 21*a(n-3); a(0)=0, a(1)=1, a(2)=0
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoization cache: cache[i] holds a(i) for 0 <= i < cache_len */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of computed elements */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Ensure capacity for indices up to n */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        /* Grow cache, doubling until enough */
        size_t new_cap = cache_cap == 0 ? 8 : cache_cap;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();             /* memory allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n (start at current cache_len) */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i == 0)
            cache[i] = (seq_t)0;
        else if (i == 1)
            cache[i] = (seq_t)1;
        else if (i == 2)
            cache[i] = (seq_t)0;
        else
            cache[i] = (seq_t)2 * cache[i - 2] + (seq_t)21 * cache[i - 3];
    }
    cache_len = needed;   /* all terms up to n are now stored */

    return cache[(size_t)n];
}
