/*
 * A181281  A version of Josephus problem: a(n) is the surviving integer under
 *          the following elimination process. Arrange 1,2,3,...,n in a
 *          circle, increasing clockwise. Starting with i=1, delete the
 *          integer 4 places clockwise from i. Repeat, counting 4 places from
 *          the next undeleted integer, until only one i...
 * https://oeis.org/A181281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1) + 4) mod n + 1, a(1) = 1
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the survivor in a Josephus problem with step size 4. */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoisation cache is static for all calls */
    static seq_t *cache = NULL;   /* cache[1] .. cache[cached] hold results */
    static idx_t  cached = 0;     /* largest n already computed */

    if (n == 0)                  /* should not happen (offset 1) */
        return (seq_t)1;

    /* initialise cache on first use */
    if (cache == NULL) {
        cache = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;     /* memory allocation failure (unlikely) */
        cache[0] = (seq_t)0;     /* unused */
        cache[1] = (seq_t)1;     /* a(1) = 1 */
        cached = 1;
    }

    /* extend cache if necessary */
    if (n > cached) {
        /* reallocate to accommodate up to n */
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            (size_t)(n + 1) * sizeof(seq_t));
        if (new_cache == NULL)
            return (seq_t)0;     /* memory allocation failure (unlikely) */
        cache = new_cache;

        /* compute missing values */
        for (idx_t i = cached + 1; i <= n; ++i) {
            /* a(i) = (a(i-1) + 4) mod i + 1      */
            cache[i] = (cache[i - 1] + (seq_t)4) % (seq_t)i + (seq_t)1;
        }
        cached = n;
    }

    return cache[n];
}
