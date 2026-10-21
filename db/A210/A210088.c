/*
 * A210088  Number of (n+1) X 2 0..2 arrays containing all values 0..2 with
 *          every 2 X 2 subblock having one or two distinct values, and new
 *          values 0..2 introduced in row major order.
 * https://oeis.org/A210088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A210088
 *  Empirical recurrence:
 *     a(1)=0, a(2)=9, a(3)=90, a(4)=657
 *     for n≥5:  a(n)=10*a(n-1) - 27*a(n-2) + 10*a(n-3) + 8*a(n-4)
 *
 *  The function computes the terms using memoisation.  All arithmetic
 *  is performed in seq_t, which may be an unsigned integer type or an
 *  arbitrary‑precision integer class.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Guard against illegal indices */
    if (n == 0) return 0;

    /* First four terms */
    static const seq_t init[4] = {(seq_t)0, (seq_t)9,
                                  (seq_t)90, (seq_t)657};

    /* Static cache for previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* allocated slots */
    static idx_t max_computed = 0; /* largest n that has been computed */

    /* Ensure the cache can hold the first four terms and all terms up
       to the requested n.  The cache is grown only when necessary. */
    idx_t needed = (n > 4) ? n : 4;
    if (needed > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * needed);
        if (tmp == NULL) return 0;      /* allocation failure – unlikely */
        cache = tmp;
        cache_size = needed;
    }

    /* Initialise the first four entries once */
    if (max_computed == 0) {
        for (idx_t i = 0; i < 4; ++i) cache[i] = init[i];
        max_computed = 4;
    }

    /* Compute terms that are still missing, using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* i is a 1‑based index.  cache[k] holds a(k+1).  */
        seq_t term = 10 * cache[i - 2];      /* 10*a(i-1) */
        term -= 27 * cache[i - 3];           /* -27*a(i-2) */
        term += 10 * cache[i - 4];           /* +10*a(i-3) */
        term += 8  * cache[i - 5];           /* +8*a(i-4)  */
        cache[i - 1] = term;
    }
    if (n > max_computed) max_computed = n;

    return cache[n - 1];
}
