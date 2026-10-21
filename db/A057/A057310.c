/*
 * A057310  Duplicate of A008084.
 * https://oeis.org/A057310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) satisfies the linear recurrence a(n) = 2*a(n-1) − 2*a(n-2)
 *            + 3*a(n-3) − 3*a(n-4) + 2*a(n-5) − 2*a(n-6) + a(n-7) for n ≥ 8,
 *            with initial values a(0)=1, a(1)=4, a(2)=9, a(3)=19, a(4)=35,
 *            a(5)=52, a(6)=72, a(7)=100
 * Range:     i8 n <= 7, i16 n <= 46, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values up to a(7) */
    static const seq_t init[8] = {1, 4, 9, 19, 35, 52, 72, 100};

    /* Cache for computed terms */
    static seq_t *cache = NULL;
    static idx_t  cap   = 0;   /* allocated capacity of cache   */
    static idx_t  last  = 0;   /* first index not yet computed  */

    /* Initialise cache on first call */
    if (cache == NULL) {
        /* allocate some initial space (at least 8) */
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        /* store the first 8 known terms */
        for (idx_t i = 0; i < 8; ++i)
            cache[i] = init[i];
        last = 8;               /* terms 0..7 are ready */
    }

    /* Return a known term immediately */
    if (n < last)
        return cache[n];

    /* Grow cache if needed */
    if (n >= cap) {
        idx_t newcap = cap;
        while (newcap <= n) newcap <<= 1;   /* double until large enough */
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap   = newcap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = last; i <= n; ++i) {
        seq_t val = (seq_t)2 * cache[i-1]
                  - (seq_t)2 * cache[i-2]
                  + (seq_t)3 * cache[i-3]
                  - (seq_t)3 * cache[i-4]
                  + (seq_t)2 * cache[i-5]
                  - (seq_t)2 * cache[i-6]
                  +      cache[i-7];
        cache[i] = val;
    }

    last = n + 1;   /* all terms up to n are now cached */
    return cache[n];
}
