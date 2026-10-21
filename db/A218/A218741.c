/*
 * A218741  a(n) = (38^n - 1)/37.
 * https://oeis.org/A218741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (38^n - 1)/37 Recurrence: a(0) = 0, a(1) = 1, a(n) =
 *            39*a(n-1) - 38*a(n-2) for n ≥ 2 Implements memoisation so that
 *            calls may occur in any order
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of cached values a(0)...a(k-1) */
static size_t cache_len = 0;     /* number of cached elements (k) */

/* Computes a(n). */
seq_t a(idx_t n)
{
    /* initialise cache with first two terms */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* allocation failure – unlikely */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
    }

    /* extend cache up to n (inclusive) if needed */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;      /* allocation failure */
        cache = new_cache;

        const seq_t mul39 = (seq_t)39;
        const seq_t mul38 = (seq_t)38;

        for (idx_t i = cache_len; i <= n; ++i) {
            /* compute a(i) = 39*a(i-1) - 38*a(i-2) */
            seq_t term1 = mul39 * cache[i - 1];
            seq_t term2 = mul38 * cache[i - 2];
            cache[i] = term1 - term2;
        }
        cache_len = new_len;
    }

    return cache[n];
}
