/*
 * A177452  Partial sums of A002055.
 * https://oeis.org/A177452
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=5..n} binomial(i-3,2) * binomial(2*i-6,i-5) / (i-4)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Partial sums of OEIS A177452 (offset 5) */

#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Undefined for indices below the offset; return 0. */
    if (n < 5)
        return 0;

    static seq_t *cache = NULL;          /* array of partial sums */
    static idx_t  cache_size = 0;        /* allocated size (max index + 1) */
    static idx_t  max_computed = 0;      /* highest index for which cache[i] is valid */

    /* Ensure array is large enough to hold index n. */
    if (cache_size <= n) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp)                     /* allocation failure */
            return 0;
        /* Zero‑initialize the newly allocated portion. */
        if (cache_size < n + 1)
            memset(tmp + cache_size, 0, (size_t)(n + 1 - cache_size) * sizeof(seq_t));
        cache = tmp;
        cache_size = n + 1;
    }

    /* Compute missing terms up to n. */
    if (max_computed < 5) {
        /* No terms computed yet.  Start at i = 5. */
        for (idx_t i = 5; i <= n; ++i) {
            seq_t term = (binomial(i - 3, 2) * binomial(2 * i - 6, i - 5))
                         / (seq_t)(i - 4);
            cache[i] = term;                     /* first term (i == 5) has no predecessor */
            if (i > 5)
                cache[i] = cache[i - 1] + term;
        }
    } else {
        /* Continue from max_computed + 1. */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t term = (binomial(i - 3, 2) * binomial(2 * i - 6, i - 5))
                         / (seq_t)(i - 4);
            cache[i] = cache[i - 1] + term;
        }
    }

    max_computed = n;
    return cache[n];
}
