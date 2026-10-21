/*
 * A220558  Number of ways to reciprocally link elements of an n X 4 array
 *          either to themselves or to exactly one horizontal or antidiagonal
 *          neighbor.
 * https://oeis.org/A220558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 17*a(n-2) + 7*a(n-3) - a(n-4); Initial
 *            values: a(1)=5, a(2)=47, a(3)=430, a(4)=3966
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises the first 10 000 terms in a static array
   (which is sufficient for the typical test ranges).  For larger n it
   falls back to an iterative computation that only keeps the last four
   terms, avoiding any dynamic allocation that would be problematic
   with an arbitrary-precision seq_t in C++. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle indices coming from zero or negative: they are unused. */
    if (n < 1)
        return (seq_t)0;

    /* If n is within the cached window, use the static cache. */
    if (n <= 10000)
    {
        static seq_t cache[10001] = {0};     /* indices 1..10000 */
        static idx_t cached_max = 0;        /* largest n already computed */

        /* Initialise base values once. */
        if (cached_max == 0)
        {
            cache[1] = (seq_t)5;
            cache[2] = (seq_t)47;
            cache[3] = (seq_t)430;
            cache[4] = (seq_t)3966;
            cached_max = 4;
        }

        /* Compute any missing terms up to n. */
        if (n > cached_max)
        {
            for (idx_t i = cached_max + 1; i <= n; ++i)
            {
                seq_t term = (seq_t)11 * cache[i - 1]
                           - (seq_t)17 * cache[i - 2]
                           + (seq_t)7  * cache[i - 3]
                           - cache[i - 4];
                cache[i] = term;
            }
            cached_max = n;
        }

        return cache[n];
    }

    /* For n > 10000, compute iteratively without storing all values. */
    /* Compute base values for indices 1..4. */
    if (n <= 4)
    {
        /* Use a small array for the first four terms. */
        static const seq_t base[5] = {(seq_t)0, (seq_t)5, (seq_t)47,
                                      (seq_t)430, (seq_t)3966};
        return base[n];
    }

    /* Iterative computation for n > 4. */
    seq_t t1 = (seq_t)5;      /* a(i-4)  */
    seq_t t2 = (seq_t)47;     /* a(i-3)  */
    seq_t t3 = (seq_t)430;    /* a(i-2)  */
    seq_t t4 = (seq_t)3966;   /* a(i-1)  */
    seq_t tn = 0;             /* a(i)    */

    for (idx_t i = 5; i <= n; ++i)
    {
        tn = (seq_t)11 * t4
          - (seq_t)17 * t3
          + (seq_t)7  * t2
          - t1;
        /* Shift window for next iteration. */
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = tn;
    }

    return tn;
}
