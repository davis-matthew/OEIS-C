/*
 * A054486  Expansion of (1+2*x)/(1-3*x+x^2).
 * https://oeis.org/A054486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2), a(0) = 1, a(1) = 5 Generates A054486.
 *            The function memoizes values so calls in any order are fast
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* Static cache for computed values. */
    static seq_t *cache = NULL;          /* Array of values */
    static size_t  cap   = 0;            /* Allocated capacity */
    static idx_t   max_cached = (idx_t)-1; /* Highest index computed */

    /* Basic base cases. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    /* Initialise cache if not yet done. */
    if (cache == NULL) {
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) { /* allocation failed – fall back to direct compute */
            seq_t a0 = 1, a1 = 5, an;
            for (idx_t i = 2; i <= n; ++i) {
                an = (seq_t)3 * a1 - a0;
                a0 = a1;
                a1 = an;
            }
            return an;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        max_cached = 1;
    }

    /* Recursively grow the cache if necessary. */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) { /* allocation failed – fall back to direct compute */
            seq_t a0 = 1, a1 = 5, an;
            for (idx_t i = 2; i <= n; ++i) {
                an = (seq_t)3 * a1 - a0;
                a0 = a1;
                a1 = an;
            }
            return an;
        }
        cache = tmp;
        cap = new_cap;
    }

    /* If the desired value is already cached, return it. */
    if (n <= max_cached) return cache[n];

    /* Compute new values up to n, storing them in the cache. */
    seq_t a0 = cache[max_cached-1];
    seq_t a1 = cache[max_cached];
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t an = (seq_t)3 * a1 - a0;
        cache[i] = an;
        a0 = a1;
        a1 = an;
    }
    max_cached = n;
    return cache[n];
}
