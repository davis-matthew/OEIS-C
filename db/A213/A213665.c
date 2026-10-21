/*
 * A213665  Number of dominating subsets of the graph G(n) obtained by joining
 *          a vertex with two consecutive vertices of the cycle graph C_n (n
 *          >= 3).
 * https://oeis.org/A213665
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n‑1) + a(n‑2) + a(n‑3) with a(3) = 13, a(4) = 23, a(5)
 *            = 43. For n < 3 the return value is undefined (0 is returned)
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 35, u64 n <= 71, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of terms up to current maximum n */
static idx_t cache_size = 0;     /* number of allocated elements */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 3; behaviour for n < 3 may be
       ignored, but we return 0 for safety. */
    if (n < 3) {
        return (seq_t)0;
    }

    /* First call: allocate minimal array and store the known initial
       values.  Indices 0,1,2 remain unused. */
    if (cache == NULL) {
        cache_size = 6;                              /* indices 0..5 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        /* populate the known initial values */
        cache[3] = (seq_t)13;
        cache[4] = (seq_t)23;
        cache[5] = (seq_t)43;
    }

    /* For n ≤ 5 we can return the pre‑computed value immediately. */
    if (n <= 5) {
        return cache[n];
    }

    /* Grow the cache if necessary and compute any missing terms by the
       3‑term recurrence. */
    if (cache_size <= n) {
        idx_t old_size = cache_size;            /* current size before grow */
        cache_size = n + 1;                     /* new required size */
        cache = (seq_t *)realloc(cache,
                                 cache_size * sizeof(seq_t));
        /* Compute terms from the first missing index up to n. */
        for (idx_t i = old_size; i <= n; ++i) {
            cache[i] = cache[i - 1] + cache[i - 2] + cache[i - 3];
        }
    }

    return cache[n];
}
