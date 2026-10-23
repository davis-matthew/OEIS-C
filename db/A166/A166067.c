/*
 * A166067  Fibonacci sequence beginning 1, 69.
 * https://oeis.org/A166067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 1, a(1) = 69
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 38, u64 n <= 85, u128 n <= 177,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

extern seq_t a(idx_t n);

/* Compute the n-th term of A166067, a modified Fibonacci sequence
 * with a(0)=1, a(1)=69.  Results are cached so subsequent calls are
 * O(1).  The cache is grown on demand. */
seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t   cache_cap = 0;   /* number of elements allocated */

    /* grow cache if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* malloc failed – for the contest we simply abort */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = 0; i <= n; ++i) {
        if (i >= cache_cap) { /* should not happen because we grew above */
            abort();
        }
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)69;
        } else {
            cache[i] = cache[i - 1] + cache[i - 2];
        }
    }

    return cache[n];
}
