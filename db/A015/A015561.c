/*
 * A015561  Expansion of x/(1 - 7*x - 4*x^2).
 * https://oeis.org/A015561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) + 4*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Expands x/(1 - 7*x - 4*x²)  */

/* This implementation uses a simple memoising array.
   It is fully portable to C11 and C++17 and requires only the
   standard library.  The helper functions mentioned in the
   problem statement are not needed. */

#include <stdlib.h>   /* malloc, realloc */
#include <stddef.h>   /* size_t */
#include <stdint.h>   /* for uint64_t if used by the build */
#include <stdbool.h>  /* for bool */

/* The types seq_t and idx_t are supplied by the build. */
seq_t a(idx_t n)
{
    /* Static cache of computed values. */
    static seq_t *cache   = NULL;  /* array of seq_t values   */
    static size_t cache_cap = 0;   /* allocated capacity      */
    static size_t cache_len = 0;   /* number of stored values */

    /* Initialisation: allocate space for the first two terms. */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* fallback on allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_cap = 2;
        cache_len = 2;
    }

    /* Return cached value if already known. */
    if ((size_t)n < cache_len)
        return cache[n];

    /* Ensure capacity large enough for indices up to n. */
    size_t needed = (size_t)(n + 1);
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 2;
        while (new_cap < needed)
            new_cap <<= 1;              /* grow by factor of 2 */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* allocation failure */
        cache       = tmp;
        cache_cap   = new_cap;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = (seq_t)7 * cache[i - 1] + (seq_t)4 * cache[i - 2];
    }

    cache_len = (size_t)(n + 1);
    return cache[n];
}
