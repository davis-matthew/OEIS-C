/*
 * A025829  Expansion of 1/((1-x^3)*(1-x^4)*(1-x^7)).
 * https://oeis.org/A025829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3)+a(n-4)-a(n-10)-a(n-11)+a(n-14)
 * Range:     u8 n <= 200, u16 n <= 3311, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0)=1, a(1)=0, a(2)=0 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* enlarge cache if necessary */
    if (n + 1 > cache_len)
    {
        idx_t new_len = n + 1;
        /* realloc handles NULL pointer case */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)
            return 0; /* allocation failure; caller will handle */
        cache = tmp;
        /* initialise new entries lazily while filling */
        /* compute from cache_len to n, inclusive */
        for (idx_t i = cache_len; i <= n; ++i)
        {
            if (i == 0)
            {
                cache[i] = (seq_t)1;
            }
            else if (i == 1 || i == 2)
            {
                cache[i] = (seq_t)0;
            }
            else
            {
                seq_t sum = (i >= 3 ? cache[i - 3] : (seq_t)0);
                sum += (i >= 4 ? cache[i - 4] : (seq_t)0);
                if (i >= 10)
                    sum -= cache[i - 10];
                if (i >= 11)
                    sum -= cache[i - 11];
                if (i >= 14)
                    sum += cache[i - 14];
                cache[i] = sum;
            }
        }
        cache_len = new_len;
    }
    return cache[n];
}
