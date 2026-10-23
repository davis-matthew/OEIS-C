/*
 * A164541  a(n) = 6*a(n-1) - a(n-2) for n > 1; a(0) = 1, a(1) = 15.
 * https://oeis.org/A164541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2); a(0)=1, a(1)=15;
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for memoizing previously computed values */
static seq_t *seq_cache = NULL;
static size_t cache_len = 0;   /* number of values stored */
static size_t cache_cap = 0;   /* allocated capacity */

seq_t a(idx_t n)
{
    /* Ensure cache is initialized with base cases */
    if (cache_len == 0) {
        cache_cap = 2;
        seq_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        seq_cache[0] = 1;
        seq_cache[1] = 15;
        cache_len = 2;
    }

    /* If the requested index exceeds current capacity, grow the array */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed; in production code you might abort or handle */
            return 0;
        }
        seq_cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing values up to index n */
    if ((size_t)(n + 1) > cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_cache[i] = (seq_t)6 * seq_cache[i - 1] - seq_cache[i - 2];
        }
        cache_len = (size_t)(n + 1);
    }

    return seq_cache[n];
}
