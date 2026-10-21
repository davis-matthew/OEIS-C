/*
 * A209808  Number of (n+1) X 3 0..3 arrays with every 2 X 2 subblock having
 *          two distinct values, and new values 0..3 introduced in row major
 *          order.
 * https://oeis.org/A209808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 17*a(n-1) - 68*a(n-2) - 56*a(n-3) + 397*a(n-4) -
 *            181*a(n-5) - 264*a(n-6) + 108*a(n-7); a(1)=34, a(2)=302,
 *            a(3)=2721, a(4)=25505, a(5)=244208, a(6)=2377378, a(7)=23398105
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base cases indexed from 1 */
    static const seq_t init[7] = {34, 302, 2721, 25505, 244208, 2377378, 23398105};

    if (n <= 0)
        return (seq_t)0;

    /* If n <= 7 just return the constant */
    if (n <= 7)
        return init[n - 1];

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;
    static idx_t   curmax = 0;

    /* Allocate or grow cache up to n */
    if (cap <= (size_t)n) {
        size_t newcap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0; /* allocation failed */
        /* initialise new entries to zero */
        for (size_t i = cap; i < newcap; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cap   = newcap;
    }

    /* Initialise base values into cache if not yet done */
    if (curmax < 7) {
        for (idx_t i = curmax + 1; i <= 7; ++i)
            cache[i] = init[i - 1];
        curmax = 7;
    }

    /* Compute missing values up to n */
    for (idx_t i = curmax + 1; i <= n; ++i) {
        /* t1 = 17*a(i-1) + 397*a(i-4) + 108*a(i-7) */
        seq_t t1 = cache[i - 1];
        t1 *= 17;
        t1 += cache[i - 4] * 397;
        t1 += cache[i - 7] * 108;

        /* t2 = 68*a(i-2) + 56*a(i-3) + 181*a(i-5) + 264*a(i-6) */
        seq_t t2 = cache[i - 2];
        t2 *= 68;
        t2 += cache[i - 3] * 56;
        t2 += cache[i - 5] * 181;
        t2 += cache[i - 6] * 264;

        cache[i] = t1 - t2;
    }

    curmax = n;
    return cache[n];
}
