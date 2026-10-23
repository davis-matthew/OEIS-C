/*
 * A170396  Number of reduced words of length n in Coxeter group on 3
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^44 = I.
 * https://oeis.org/A170396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of reduced words of length n in the Coxeter group
 *            on 3 generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 *            The generating function of the sequence is (t^44 + 2*t^43 +
 *            2*t^42 + … + 2*t + 1) / (t^44 - t^43 - t^42 - … - t - 1). This
 *            implies the following linear recurrence: For 0 ≤...
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t   *cache    = NULL;   /* array of computed values   */
    static idx_t   capacity = 0;        /* allocated capacity         */
    static idx_t   computed = 0;        /* highest index computed     */
    static seq_t   cum_sum  = 0;        /* sum of all computed terms  */
    static seq_t   window_sum = 0;      /* sum of last 44 terms       */

    /* initialise cache with a(0) on first call */
    if (cache == NULL)
    {
        capacity = (n + 1) >= 100 ? n + 1 : 100;
        cache = (seq_t *)malloc(sizeof(seq_t) * capacity);
        cache[0] = (seq_t)1;
        cum_sum  = (seq_t)1;
        computed = 0;
    }

    /* extend capacity if necessary */
    if (n >= capacity)
    {
        idx_t new_cap = capacity;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        capacity = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t m = computed + 1; m <= n; ++m)
    {
        seq_t val;

        if (m <= 43)               /* 1 ≤ m ≤ 43 :   p(m)=2   */
        {
            val = cum_sum + (seq_t)2;
        }
        else if (m == 44)          /* m = 44 :   p(44)=1      */
        {
            val = cum_sum + (seq_t)1;
        }
        else                       /* m ≥ 45 :  homogeneous  */
        {
            val = window_sum;
        }

        cache[m] = val;
        cum_sum  = cum_sum + val;

        if (m >= 44)               /* maintain window_sum for n ≥ 45 */
        {
            if (m == 44)            /* initial window sum after a(44)  */
            {
                window_sum = (seq_t)0;
                for (idx_t i = 1; i <= 44; ++i)
                    window_sum += cache[i];
            }
            else                    /* slide the window   */
                window_sum = window_sum + val - cache[m - 44];
        }
    }

    computed = n;
    return cache[n];
}
