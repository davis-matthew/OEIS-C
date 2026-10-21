/*
 * A227145  Numbers satisfying an infinite nested recurrence relation.
 * https://oeis.org/A227145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - 1 - a(n-1) - a(a(n-2)) - a(a(a(n-3))) -
 *            a(a(a(a(n-4)))) ... with a(n) = 0 for n <= 1. The summation
 *            stops automatically when the nested value becomes 0, since all
 *            further terms are then 0
 * Range:     u8 n <= 671, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoization cache: cache[i] = a(i).  Indices 0 and 1 are 0. */
    static seq_t *cache = NULL;
    static idx_t cap = 0;          /* allocated size of cache */
    static idx_t max_computed = 0; /* largest n for which cache[i] is valid */

    /* Base case for n <= 1. */
    if (n <= 1)
        return (seq_t)0;

    /* Ensure cache is large enough. */
    if (n + 1 > cap) {
        idx_t new_cap = (cap == 0) ? 2 : cap;
        while (new_cap < n + 1)
            new_cap <<= 1;         /* grow exponentially */

        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;       /* allocation failure – return 0 */

        /* Copy existing values. */
        for (idx_t i = 0; i < cap; ++i)
            new_cache[i] = cache[i];

        /* Initialise new entries to 0. */
        for (idx_t i = cap; i < new_cap; ++i)
            new_cache[i] = (seq_t)0;

        free(cache);
        cache = new_cache;
        cap = new_cap;
    }

    /* Initialize the first two values once. */
    if (max_computed == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        max_computed = 1;
    }

    /* Compute any missing values up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t val = (seq_t)i - (seq_t)1;          /* leading n - 1 term */

        /* Subtract nested recurrence terms. */
        for (idx_t k = 1; k < i; ++k) {
            /* Starting index for this nest. */
            idx_t y = i - k;
            seq_t cur = cache[y];

            /* Apply a() (k-1) additional times. */
            for (idx_t depth = 1; depth < k; ++depth) {
                /* cur already holds a(a(...(y))) from previous depth */
                cur = cache[(idx_t)cur];
            }

            val -= cur;
            if (cur == (seq_t)0)
                break;                     /* further terms will also be 0 */
        }

        cache[i] = val;
    }

    max_computed = n;
    return cache[n];
}
