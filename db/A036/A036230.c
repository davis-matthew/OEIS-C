/*
 * A036230  a(n+1) = a(n) + sum of digits of a(n) starting with 110.
 * https://oeis.org/A036230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 110, a(n) = a(n-1) + digit_sum(a(n-1))   for n >= 2 */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"

/* Computes the n-th term of OEIS sequence A036230 */
seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* First call – initialise cache with a(1) */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – unlikely */
        cache[0] = (seq_t)110;        /* a(1) */
        cache_len = 1;
    }

    /* Expand cache if requested index exceeds current length */
    if ((idx_t)n > cache_len) {
        /* Allocate larger array */
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * (size_t)n);
        if (!new_cache) return cache[cache_len - 1]; /* fallback on failure */

        /* Copy existing values */
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];

        /* Compute missing terms */
        for (idx_t i = cache_len; i < (idx_t)n; ++i) {
            seq_t prev = new_cache[i - 1];
            seq_t dsum = digit_sum(prev);
            new_cache[i] = prev + dsum;
        }

        /* Replace old cache */
        free(cache);
        cache = new_cache;
        cache_len = (idx_t)n;
    }

    /* Return the requested term (1‑indexed) */
    return cache[n - 1];
}
