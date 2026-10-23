/*
 * A170520  Number of reduced words of length n in Coxeter group on 31
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^46 = I.
 * https://oeis.org/A170520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   recurrence derived from the generating function a(0) = 1 for 1
 *            <= n <= 45: a(n) = 2 + 29 * sum_{k=0}^{n-1} a(k) for n >= 46:
 *            a(n) = 29 * sum_{k=1}^{45} a(n-k) - 435 * a(n-46) The first 46
 *            terms are obtained directly from the rational generating
 *            function and then the recurrence is applied
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t len    = 0;   /* number of values stored in cache   */
    static seq_t sum_last45 = 0;  /* sum of the last 45 values currently in cache */

    /* first call: initialise cache with the first 46 terms */
    if (cache == NULL)
    {
        cap = 64;                     /* initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;          /* allocation failure */

        /* term 0 */
        cache[0] = (seq_t)1;
        len = 1;

        seq_t prefix = (seq_t)1;     /* sum of a[0] .. a[i-1] during the loop */

        /* terms 1 .. 45 : a(n) = 2 + 29 * prefix */
        for (idx_t i = 1; i <= 45; ++i)
        {
            cache[i] = (seq_t)2 + (seq_t)29 * prefix;
            prefix += cache[i];
            ++len;
        }

        /* sum_last45 = sum of a[1] .. a[45] */
        sum_last45 = prefix - cache[0];

        /* term 46 : a(n) = 29 * sum_last45 - 435 * a(0) */
        cache[46] = (seq_t)29 * sum_last45 - (seq_t)435 * cache[0];
        ++len;

        /* update sum_last45 for the next step: drop a[1] */
        sum_last45 += cache[46] - cache[1];
    }

    /* extend cache up to index n if required */
    if ((idx_t)len <= n)
    {
        for (idx_t k = len; k <= n; ++k)
        {
            /* ensure capacity */
            if (k >= cap)
            {
                cap = cap * 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                if (cache == NULL)
                    return (seq_t)0;
            }

            /* recurrence: a(k) = 29 * sum_last45 - 435 * a(k-46) */
            seq_t next = (seq_t)29 * sum_last45 - (seq_t)435 * cache[k - 46];
            cache[k] = next;
            ++len;

            /* update sum_last45 for the next iteration:
               drop a(k-45), add the new term */
            sum_last45 += next - cache[k - 45];
        }
    }

    return cache[n];
}
