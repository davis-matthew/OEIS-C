/*
 * A212339  Sequence of coefficients of x in marked mesh pattern generating
 *          function Q_{n,132}^(0,0,3,0)(x).
 * https://oeis.org/A212339
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the recurrence
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 3*a(n-1) +      a(n-3) -12*a(n-4) - 3*a(n-5) + a(n-6) + 18*a(n-7) + 12*a(n-8) + 8*a(n-9) */
/* Initial values (for n = 4 .. 12): 5, 19, 61, 188, 523, 1387, 3565, 8888, 21674 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;  /* maximum index currently stored in cache */

    /* below offset behaviour irrelevant */
    if (n < 4)
        return 0;

    /* initialise cache with known terms up to n = 12 */
    if (!cache)
    {
        /* allocate indices 0 .. 12 inclusive */
        const idx_t init_size = 13;            /* 0..12 */
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (!cache)
            return 0; /* malloc failure */

        /* zero out indices 0..3 (unused) */
        for (idx_t i = 0; i < 4; ++i)
            cache[i] = (seq_t)0;

        /* set official initial terms */
        cache[4] = (seq_t)5;
        cache[5] = (seq_t)19;
        cache[6] = (seq_t)61;
        cache[7] = (seq_t)188;
        cache[8] = (seq_t)523;
        cache[9] = (seq_t)1387;
        cache[10] = (seq_t)3565;
        cache[11] = (seq_t)8888;
        cache[12] = (seq_t)21674;

        max_computed = 12;
    }

    /* extend cache if needed */
    if (n > max_computed)
    {
        /* new size must accommodate indices 0 .. n */
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return 0; /* realloc failure */

        cache = tmp;

        /* compute new terms using the recurrence */
        for (idx_t i = max_computed + 1; i <= n; ++i)
        {
            /* recurrence applies only for i >= 13; terms for i <= 12 are preset */
            if (i <= 12)
                continue;

            /* sum of positive contributions */
            seq_t pos = (seq_t)3 * cache[i - 1]
                      + (seq_t)1 * cache[i - 3]
                      + (seq_t)1 * cache[i - 6]
                      + (seq_t)18 * cache[i - 7]
                      + (seq_t)12 * cache[i - 8]
                      + (seq_t)8 * cache[i - 9];

            /* sum of negative contributions */
            seq_t neg = (seq_t)12 * cache[i - 4]
                      + (seq_t)3 * cache[i - 5];

            cache[i] = pos - neg;  /* pos >= neg for valid sequence */
        }
        max_computed = n;
    }

    return cache[n];
}
