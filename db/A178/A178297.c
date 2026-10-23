/*
 * A178297  Number of collinear point 6-tuples in a 7 X 7 X 7 X...
 *          n-dimensional cubic grid.
 * https://oeis.org/A178297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 27*a(n-1) - 239*a(n-2) + 693*a(n-3), a(0)=0, a(1)=7,
 *            a(2)=116
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) with memoization in O(n) time. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* store already computed terms   */
    static size_t cache_len = 0;     /* number of terms stored (max index + 1) */

    /* Initialise cache with the first three values if not already done. */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* unlikely, but safe */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)116;
    }

    /* Return a value that has already been computed. */
    if ((size_t)n < cache_len) {
        return cache[(size_t)n];
    }

    /* Extend the cache to contain up to index n. */
    size_t needed = n + 1;
    size_t old_len = cache_len;
    size_t new_cap = old_len ? old_len : 1;
    while (new_cap < needed) {
        new_cap <<= 1;                       /* grow by factor of 2 */
    }

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return (seq_t)0;               /* allocation failure */
    cache = tmp;
    cache_len = new_cap;                     /* capacity for future grows */

    /* Compute missing terms using the recurrence. */
    for (size_t i = old_len; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t term = 27 * a1 + 693 * a3;     /* 27*a(i-1) + 693*a(i-3) */
        term -= 239 * a2;                    /* -239*a(i-2) */
        cache[i] = term;
    }

    cache_len = needed;                      /* now we have terms 0..n */
    return cache[(size_t)n];
}
