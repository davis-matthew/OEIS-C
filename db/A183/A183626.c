/*
 * A183626  Number of (n+1) X 4 0..2 arrays with every 2 X 2 subblock summing
 *          to 4.
 * https://oeis.org/A183626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=115, a(2)=189, a(3)=355 * Recurrence: a(n) = 6*a(n-1) -
 *            11*a(n-2) + 6*a(n-3) for n ≥ 4
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case (not part of the OEIS offset) */
    if (n < 1) return 0;

    /* Static cache of previously computed terms: cache[i] holds a(i+1) */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last = 0;

    /* First invocation: initialise cache for the first three terms */
    if (cache == NULL) {
        cap  = 3;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;            /* in case malloc failed */
        cache[0] = 115;                  /* a(1) */
        cache[1] = 189;                  /* a(2) */
        cache[2] = 355;                  /* a(3) */
        last = 3;                        /* highest index computed */
    }

    /* Return cached value if already computed */
    if (n <= last)
        return cache[n - 1];

    /* Grow the cache if necessary */
    if ((idx_t)cap < n) {
        seq_t *newc = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!newc) return 0;            /* allocation failure */
        cache = newc;
        cap   = n;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* a(i) = 6*a(i-1) - 11*a(i-2) + 6*a(i-3) */
        seq_t term1 = cache[i - 2] * (seq_t)6;   /* 6*a(i-1) */
        seq_t term2 = cache[i - 3] * (seq_t)11;  /* 11*a(i-2) */
        seq_t term3 = cache[i - 4] * (seq_t)6;   /* 6*a(i-3) */
        cache[i - 1] = term1 - term2 + term3;
    }
    last = n;
    return cache[n - 1];
}
