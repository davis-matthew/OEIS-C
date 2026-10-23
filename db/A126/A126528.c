/*
 * A126528  Number of base 7 n-digit numbers with adjacent digits differing by
 *          five or less.
 * https://oeis.org/A126528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-1) + 5*a(n-2); a(0)=1, a(1)=7 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle base cases first */
    if (n == 0) return 1;
    if (n == 1) return 7;

    /* Static cache for previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Initialise cache with the first two terms when needed */
    if (cache == NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (cache == NULL) return 0;          /* allocation failed */
        cache[0] = 1;
        cache[1] = 7;
        cache_len = 2;
    }

    /* Extend cache if the requested index is larger than any cached value */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (new_cache == NULL) return 0;      /* allocation failed */

        cache = new_cache;
        /* Compute missing terms using the recurrence */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            cache[i] = 6 * cache[i - 1] + 5 * cache[i - 2];
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
