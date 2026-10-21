/*
 * A234597  Number of Weyl group elements, not containing an s_1 factor, which
 *          contribute nonzero terms to Kostant's weight multiplicity formula
 *          when computing the multiplicity of the zero-weight in the adjoint
 *          representation for the Lie algebra of type D and rank n.
 * https://oeis.org/A234597
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 3*a(n-3) + a(n-4) initial values: a(4)
 *            = 5, a(5) = 11, a(6) = 21, a(7) = 48 (offset 4; for n < 4 the
 *            function returns 0)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 30, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Local static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t max_cached = 3;        /* highest index already computed */
    static size_t capacity = 0;         /* size of allocated cache (in elements) */

    if (n < 4) {           /* sequence not defined below the offset */
        return (seq_t)0;
    }

    /* Allocate and initialise cache on first use */
    if (cache == NULL) {
        /* Ensure capacity for indices up to n and also for the first
           few base entries (4..7).  Use the larger of (n+1) and 8. */
        capacity = (size_t)((n + 1) > 8 ? (n + 1) : 8);
        cache = (seq_t *)malloc(sizeof(seq_t) * capacity);
        if (cache == NULL) {
            /* Allocation failed – return 0 as fallback (should not happen). */
            return (seq_t)0;
        }
        /* Initialise all entries to 0 first (implicit). */
        /* Set the base values for indices 4..7 */
        cache[4] = (seq_t)5;
        cache[5] = (seq_t)11;
        cache[6] = (seq_t)21;
        cache[7] = (seq_t)48;
        max_cached = 7;
    }

    /* If we already have the desired value, return it immediately */
    if (n <= max_cached) {
        return cache[n];
    }

    /* Ensure the cache is large enough for indices up to n */
    if ((size_t)(n + 1) > capacity) {
        capacity = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * capacity);
        if (cache == NULL) {
            /* Reallocation failed – return 0 as fallback. */
            return (seq_t)0;
        }
    }

    /* Compute missing terms sequentially up to n using the recurrence. */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        cache[i] =
            cache[i - 1]          /* a(i-1) */
          + cache[i - 2]          /* a(i-2) */
          + (cache[i - 3] * (seq_t)3)   /* 3*a(i-3) */
          + cache[i - 4];          /* a(i-4) */
    }

    max_cached = n;
    return cache[n];
}
