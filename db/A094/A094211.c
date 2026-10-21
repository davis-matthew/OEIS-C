/*
 * A094211  a(n) = Sum_{k=0..n} binomial(7*n,7*k).
 * https://oeis.org/A094211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 71*a(n-1) + 7585*a(n-2) - 36991*a(n-3) - 128*a(n-4);
 *            a(0)=1, a(1)=2, a(2)=3434, a(3)=232562
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Small constant values for n < 4. */
    if (n < 4)
        switch ((int)n)
        {
            case 0: return (seq_t)1;
            case 1: return (seq_t)2;
            case 2: return (seq_t)3434;
            case 3: return (seq_t)232562;
        }

    /* Lazy memoisation of the recurrence. */
    static seq_t *cache      = NULL;        /* Dynamically resizeable array of terms. */
    static size_t  cached_len = 0;          /* Number of values currently stored.          */
    static size_t  cached_cap = 0;          /* Allocated capacity.                        */

    /* Initialise the cache with the known first four terms. */
    if (cache == NULL)
    {
        cached_cap = 16;                                            /* Initial size. */
        cache = (seq_t *)malloc(cached_cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3434;
        cache[3] = (seq_t)232562;
        cached_len = 4;
    }

    /* Grow the cache until it contains a(n). */
    while (cached_len <= n)
    {
        if (cached_len >= cached_cap)
        {
            size_t new_cap = cached_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            cache = tmp;
            cached_cap = new_cap;
        }

        idx_t i = cached_len;                          /* next index to compute   */

        /* Recurrence: a(n) = 71*a(n-1) + 7585*a(n-2) - 36991*a(n-3) - 128*a(n-4). */
        seq_t t1 = cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t t3 = cache[i - 3];
        seq_t t4 = cache[i - 4];

        seq_t left  = 71 * t1 + 7585 * t2;
        seq_t right = 36991 * t3 + 128 * t4;
        cache[i] = left - right;                           /* left >= right for all n >= 4 */

        ++cached_len;
    }

    return cache[n];
}
