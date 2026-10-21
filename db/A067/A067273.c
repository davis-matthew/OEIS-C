/*
 * A067273  a(n) = n*(a(n-1)*2+1), a(0) = 0.
 * https://oeis.org/A067273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0; a(n) = n * (2*a(n-1) + 1)  for n >= 1 */

#include <stdlib.h>

/* Compute the n-th term of OEIS sequence A067273.
 * The function memoizes computed values to avoid recomputation
 * across multiple calls.  It works for any build where `seq_t`
 * supports the usual integer operators. */
seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache     = NULL;     /* pointer to the array of terms */
    static size_t cache_cap = 0;        /* current allocated capacity */
    static idx_t  next      = 1;        /* index of first uncomputed term */

    /* Convenient local aliases */
    idx_t i;
    size_t idx;

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* on allocation failure return 0 */
        cache[0] = (seq_t)0;            /* a(0) = 0 */
        cache_cap = 1;
        next = 1;                       /* next term to compute is a(1) */
    }

    /* Ensure capacity for up to n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap <= (size_t)n + 1)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* on failure return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n */
    for (i = next; i <= n; ++i) {
        /* Use seq_t for all intermediates to avoid overflow in 64‑bit builds. */
        seq_t prev = cache[i - 1];
        seq_t term = (seq_t)i * ((prev * (seq_t)2) + (seq_t)1);
        cache[i] = term;
    }
    next = n + 1;   /* All terms up to n are now computed */

    return cache[n];
}
