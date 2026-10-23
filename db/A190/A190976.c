/*
 * A190976  a(n) = 8*a(n-1) - 3*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A190976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 3*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Function to compute OEIS sequence A190976 */
seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;      /* array of seq_t values */
    static size_t cache_len = 0;     /* number of valid entries in cache */

    /* Ensure the base cases are available */
    if (n < 2) {
        if (n == 0) return (seq_t)0;
        return (seq_t)1;
    }

    /* Allocate minimal cache on first call (2 entries for a(0), a(1)) */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure falls back to 0 */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
    }

    /* Grow the cache if the requested index is beyond current length */
    if (n >= cache_len) {
        /* Reallocate to accommodate n+1 entries */
        size_t new_len = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failure */
        cache = new_cache;

        /* Compute missing terms up to n */
        for (size_t k = cache_len; k <= (size_t)n; ++k) {
            /* recurrence: a(k) = 8*a(k-1) - 3*a(k-2) */
            seq_t a1 = cache[k - 1];
            seq_t a2 = cache[k - 2];
            cache[k] = (seq_t)((seq_t)8 * a1 - (seq_t)3 * a2);
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
