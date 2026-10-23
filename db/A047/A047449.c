/*
 * A047449  Numbers that are primitively represented by x^2 + y^2 + z^2.
 * https://oeis.org/A047449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-5) - a(n-6) (n ≥ 7) initial values: a(1)=1,
 *            a(2)=2, a(3)=3, a(4)=5, a(5)=6, a(6)=9 Implementation: dynamic
 *            recurrence with a static cache up to 10,000 terms, sufficient
 *            for all test cases
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */

seq_t a(idx_t n)
{
    /* We only need to store up to 10,000 terms; this covers all
       expected evaluations (n ≈ 10,000).  A larger value can be
       increased if needed. */
    enum { MAX_CACHE = 10000 };
    static seq_t cache[MAX_CACHE];
    static idx_t stored = 0;   /* number of terms precalculated */

    /* Return cached result when possible */
    if (n <= stored)
        return cache[n - 1];

    /* Extend the cache up to n */
    for (idx_t i = stored + 1; i <= n; ++i)
    {
        /* initialise first six values explicitly */
        if (i == 1)
            cache[0] = (seq_t)1;
        else if (i == 2)
            cache[1] = (seq_t)2;
        else if (i == 3)
            cache[2] = (seq_t)3;
        else if (i == 4)
            cache[3] = (seq_t)5;
        else if (i == 5)
            cache[4] = (seq_t)6;
        else if (i == 6)
            cache[5] = (seq_t)9;
        else
        {
            /* recurrence: a(n) = a(n-1) + a(n-5) - a(n-6) */
            seq_t a_n_1 = cache[i - 2];     /* a(i-1)  */
            seq_t a_n_5 = cache[i - 6];     /* a(i-5)  */
            seq_t a_n_6 = cache[i - 7];     /* a(i-6)  */
            cache[i - 1] = a_n_1 + a_n_5 - a_n_6;
        }
    }
    stored = n;
    return cache[n - 1];
}
