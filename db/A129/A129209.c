/*
 * A129209  Fourth sequence in solution to congruent number 5 problem.
 * https://oeis.org/A129209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n+2)*a(n-2) = -144*a(n+1)*a(n-1) + 2257*a(n)^2, * a(-n) = a(n)
 *            * Initial values: a(0)=1, a(1)=3, a(2)=31, a(3)=5283
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 4, i64 n <= 6, i128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* simple memoising cache */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise seed values */
    if (cache_len == 0)
    {
        cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)31;
        cache[3] = (seq_t)5283;
    }

    /* extend cache if needed */
    if (n >= cache_len)
    {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        for (idx_t k = cache_len; k <= n; ++k)
        {
            if (k <= 3) continue;         /* already seeded */

            /* recurrence: let m = k-2  (m >= 2) */
            idx_t m = k - 2;

            seq_t t1 = cache[m + 1];   /* a(m+1) */
            seq_t t2 = cache[m - 1];   /* a(m-1) */
            seq_t t3 = cache[m];       /* a(m)   */

            seq_t rhs = -(seq_t)144 * t1 * t2 + (seq_t)2257 * t3 * t3;
            seq_t denom = cache[m - 2]; /* a(m-2) */

            cache[k] = rhs / denom;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
