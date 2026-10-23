/*
 * A168945  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (OEIS style):
   a(0) = 1
   a(n) = (n==0?1:2) + 26*∑_{i=0}^{n-1} a(i)          for 1 ≤ n ≤ 21
   a(n) = 351*a(n-22) – 26*∑_{k=1}^{21} a(n-k)        for n ≥ 22   */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoised values */
    static seq_t *cache = NULL;   /* array of a(0)…a(len-1)       */
    static size_t cap   = 0;      /* allocated size                */
    static size_t len   = 0;      /* number of computed terms      */
    /* running sums for the recurrences */
    static seq_t sum_prefix = 0;  /* ∑_{i=0}^{len-1} a(i)         */
    static seq_t sum21 = 0;       /* ∑_{i=len-21}^{len-1} a(i)    */

    /* initialise the cache if required */
    if (!cache)
    {
        cap = 64;
        cache = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        len = 0;
    }

    /* extend the cache up to index n */
    while (len <= n)
    {
        /* grow the array if needed */
        if (len >= cap)
        {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cap = new_cap;
        }

        /* --- compute a[len] --- */
        if (len <= 21)          /* simple prefix recurrence */
        {
            seq_t val;
            if (len == 0)
                val = 1;
            else
                val = (seq_t)2 + sum_prefix * (seq_t)26;
            cache[len] = val;
            sum_prefix += val;   /* update cumulative sum */
        }
        else                     /* linear recurrence of order 22 */
        {
            /* initialise sum21 when we first cross index 22 */
            if (len == 22)
            {
                sum21 = 0;
                for (size_t i = 1; i <= 21; ++i)
                    sum21 += cache[i];
            }

            /* recurrence: a(n) = 351*a(n-22) - 26*sum21 */
            seq_t val = cache[len-22] * (seq_t)351 - sum21 * (seq_t)26;
            cache[len] = val;

            /* shift sum21: remove the term that leaves the 21‑term window
               and add the new term just computed. */
            sum21 = sum21 - cache[len-21] + val;
        }

        ++len;
    }

    return cache[n];
}
