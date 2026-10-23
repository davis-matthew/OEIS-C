/*
 * A017857  Expansion of 1/(1 - x^7 - x^8).
 * https://oeis.org/A017857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-7) + a(n-8) for n >= 8, with initial values a(0)=1,
 *            a(1)=a(2)=a(3)=a(4)=a(5)=a(6)=0, a(7)=1
 * Range:     u8 n <= 80, u16 n <= 140, u32 n <= 260, u64 n <= 500,
 *            u128 n <= 980, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;          /* cache array */
    static size_t cache_cap = 0;          /* current allocation size */
    static idx_t max_computed = 0;       /* highest index with a valid value */

    /* Grow the cache to at least n+1 elements */
    if (cache == NULL) {
        /* At least 8 entries are required for the base values 0..7 */
        cache_cap = (n + 1 > 8) ? (n + 1) : 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Initialise base values on first use */
    if (max_computed == 0) {
        cache[0] = (seq_t)1;
        for (idx_t i = 1; i <= 6; ++i)
            cache[i] = (seq_t)0;
        cache[7] = (seq_t)1;
        max_computed = 7;
    }

    /* Compute missing values up to n using the recurrence */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* a(i) = a(i-7) + a(i-8)  (i >= 8 by construction) */
            cache[i] = cache[i - 7] + cache[i - 8];
        }
        max_computed = n;
    }

    return cache[n];
}
