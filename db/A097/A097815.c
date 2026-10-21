/*
 * A097815  E.g.f. exp(4x)/(1-4x).
 * https://oeis.org/A097815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1 * a(n) = 4*n*a(n-1) + 4^n for n > 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lazy memoisation guarantees O(1) per call after the first n. */

#include <stddef.h>
#include <stdlib.h>

/* The types idx_t and seq_t are defined by the build system. */

seq_t a(idx_t n)
{
    /* Static cache: cache[i] holds a(i). */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Current power of 4: pow4 == 4^pow4_index. */
    static seq_t pow4   = (seq_t)1;
    static idx_t pow4_index = 0;

    if (n >= (idx_t)cache_size) {
        /* Grow the cache to accommodate index n. */
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                /* fatal if allocation fails */
        cache = tmp;

        /* Initialise the first value if the cache was empty. */
        if (cache_size == 0) {
            cache[0] = (seq_t)1;          /* a(0) = 1 */
            cache_size = 1;
        }

        /* Compute all missing terms up to n (inclusive). */
        for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
            /* Ensure pow4 == 4^i. */
            while (pow4_index < i) {
                pow4 *= (seq_t)4;
                ++pow4_index;
            }

            seq_t val = cache[(size_t)(i - 1)];
            val *= (seq_t)4;              /* multiply by 4 */
            val *= (idx_t)i;              /* multiply by i */
            val += pow4;                  /* add 4^i */

            cache[(size_t)i] = val;
        }
        cache_size = (size_t)(n + 1);
    }

    return cache[(size_t)n];
}
