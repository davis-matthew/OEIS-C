/*
 * A193668  a(n) = Sum_{i=0..n-1} (n+i)*a(n-1-i) for n>1, a(0)=1, a(1)=1.
 * https://oeis.org/A193668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A193668
 *
 * Formula:  a(0) = 1,  a(1) = 1
 *           a(n) = Sum_{i=0}^{n-1} (n+i)*a(n-1-i)   for n > 1
 *
 * Equivalent three‑term recurrence (valid for n > 2):
 *           a(n) = (n+2)*a(n-1) - (n-2)*a(n-2)
 */

#include <stdlib.h>

/* Function definition ----------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Static cache of previously computed values */
    static seq_t *cache = NULL;       /* allocated array of seq_t */
    static size_t allocated = 0;      /* number of slots allocated   */
    static idx_t max_computed = 0;    /* largest n that has been computed */

    /* First call: initialise the cache for indices 0 and 1 */
    if (allocated == 0) {
        allocated = 2;
        cache = (seq_t *)malloc(allocated * sizeof(seq_t));
        if (!cache) abort();               /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        max_computed = 1;
    }

    /* If the requested value is already known, return it promptly */
    if (n <= max_computed) {
        return cache[(size_t)n];
    }

    /* Grow the cache array if necessary to contain index n */
    if (allocated <= n) {
        size_t new_alloc = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
        if (!new_cache) abort();           /* reallocation failure */
        cache = new_cache;
        allocated = new_alloc;
    }

    /* Compute new sequence values from max_computed+1 up to n */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        if (k == 2) {
            /* Direct computation from the definition for a(2) */
            cache[(size_t)k] = (seq_t)(k + 0) * cache[(size_t)(k - 1)]
                             + (seq_t)(k + 1) * cache[(size_t)(k - 2)];
        } else { /* k >= 3, use the three‑term recurrence */
            cache[(size_t)k] =
                (seq_t)(k + 2) * cache[(size_t)(k - 1)] -
                (seq_t)(k - 2) * cache[(size_t)(k - 2)];
        }
    }

    max_computed = n;
    return cache[(size_t)n];
}
