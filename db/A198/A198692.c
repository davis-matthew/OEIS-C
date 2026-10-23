/*
 * A198692  a(n) = 11*7^n-1.
 * https://oeis.org/A198692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*7^n - 1 Recurrence: a(0) = 10, a(n) = 7*a(n-1) + 6 (n
 *            > 0)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A198692.
   Memoises previously computed values for efficiency. */

#include <stdlib.h>  /* for malloc, realloc */

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Static cache for memoised values */
    static seq_t *cache = NULL;   /* allocated cache array */
    static size_t capacity = 0;   /* allocated capacity (in elements) */
    static idx_t   max_cached = 0;/* next index to compute (cache holds 0..max_cached-1) */

    /* Ensure cache is allocated at least of size n+1 */
    if (n + 1 > capacity) {
        /* Choose new capacity slightly larger to avoid frequent reallocs */
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed – return 0 as safe fallback */
            return (seq_t)0;
        }
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute values up to n if not already cached */
    if (max_cached == 0) {
        /* Initialise first term a(0) = 10 */
        cache[0] = (seq_t)10;
        max_cached = 1;
    }

    while (max_cached <= n) {
        /* a(i) = 7*a(i-1) + 6 */
        cache[max_cached] = cache[max_cached - 1] * (seq_t)7 + (seq_t)6;
        ++max_cached;
    }

    return cache[n];
}
