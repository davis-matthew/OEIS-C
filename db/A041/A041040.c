/*
 * A041040  Numerators of continued fraction convergents to sqrt(26).
 * https://oeis.org/A041040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 10*a(n-1) + a(n-2), a(0) = 5, a(1) = 51
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* allocated array size */
    static idx_t cache_len  = 0;   /* number of terms stored (max index + 1) */

    /* Handle the first two terms directly */
    if (n == 0) return 5;
    if (n == 1) return 51;

    /* Initialise cache if not already done */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;          /* fallback if allocation fails */
        cache[0] = 5;
        cache[1] = 51;
        cache_len = 2;
    }

    /* Return cached value if available */
    if (n < cache_len) return cache[n];

    /* Grow the cache if necessary */
    if (cache_size <= n) {
        idx_t new_size = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;          /* fallback */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = cache[i-1] * 10 + cache[i-2];
    }
    cache_len = n + 1;

    return cache[n];
}
