/*
 * A223408  4 X 4 X 4 triangular graph without horizontal edges coloring a
 *          rectangular array: number of n X 1 0..9 arrays where 0..9 label
 *          nodes of a graph with edges 0,1 0,2 1,3 1,4 2,4 2,5 3,6 3,7 4,7
 *          4,8 5,8 5,9 and every array movement to a horizontal or vertical
 *          neighbor moves along an edge of this graph.
 * https://oeis.org/A223408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-2) - 12*a(n-4) + 4*a(n-6) for n ≥ 7 Initial values:
 *            a(1)=10, a(2)=24, a(3)=66, a(4)=176, a(5)=486, a(6)=1312
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 42, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static 1‑indexed cache; cache[0] is unused          */
    static seq_t *cache = NULL;
    static size_t  cache_cap = 0;   /* allocated size (≥ max index + 1) */
    static idx_t   max_cached = 0;  /* largest n that has been computed */

    if (n < 1)
        return 0;   /* undefined below the offset */

    /* Allocate or expand cache if necessary.  Always keep at least 7
       entries to hold a(1)…a(6).                                         */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 7) new_cap = 7;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)  /* allocation failed – return 0 (undefined) */
            return 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first six terms on the first call. */
    if (max_cached == 0) {
        cache[1] = (seq_t)10;
        cache[2] = (seq_t)24;
        cache[3] = (seq_t)66;
        cache[4] = (seq_t)176;
        cache[5] = (seq_t)486;
        cache[6] = (seq_t)1312;
        max_cached = 6;
    }

    /* Compute any missing terms up to n. */
    for (idx_t k = max_cached + 1; k <= n; ++k) {
        cache[k] = (seq_t)9   * cache[k-2]
                 - (seq_t)12 * cache[k-4]
                 + (seq_t)4   * cache[k-6];
    }
    if (n > max_cached)
        max_cached = n;

    return cache[n];
}
