/*
 * A049428  Row sums of triangle A049411.
 * https://oeis.org/A049428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)+5*(n-1)*a(n-2)+10*(n-1)*(n-2)*a(n-3)+
 *            10*(n-1)*(n-2)*(n-3)*a(n-4)+5*(n-1)*(n-2)*(n-3)*(n-4)*a(n-5)+
 *            (n-5)*(n-1)*(n-2)*(n-3)*(n-4)*a(n-6) with initial values
 *            a(0)=1,a(1)=1,a(2)=6,a(3)=36,a(4)=246,a(5)=2046
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_capacity = 0;

    if (n >= cache_size)
    {
        /* need to expand storage if necessary */
        idx_t needed = n + 1;
        if (needed > cache_capacity)
        {
            idx_t new_cap = cache_capacity ? cache_capacity : 16;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            /* In a correct build, realloc never fails for the given limits. */
            cache = tmp;
            cache_capacity = new_cap;
        }

        /* compute all new terms */
        for (idx_t i = cache_size; i <= n; ++i)
        {
            if (i == 0) cache[0] = 1;
            else if (i == 1) cache[1] = 1;
            else if (i == 2) cache[2] = 6;
            else if (i == 3) cache[3] = 36;
            else if (i == 4) cache[4] = 246;
            else if (i == 5) cache[5] = 2046;
            else
            {
                seq_t val = cache[i-1];
                val += (seq_t)5 * (seq_t)(i-1) * cache[i-2];
                val += (seq_t)10 * (seq_t)(i-1) * (seq_t)(i-2) * cache[i-3];
                val += (seq_t)10 * (seq_t)(i-1) * (seq_t)(i-2) * (seq_t)(i-3) * cache[i-4];
                val += (seq_t)5  * (seq_t)(i-1) * (seq_t)(i-2) * (seq_t)(i-3) * (seq_t)(i-4) * cache[i-5];
                val += (seq_t)(i-5) * (seq_t)(i-1) * (seq_t)(i-2) * (seq_t)(i-3) * (seq_t)(i-4) * cache[i-6];
                cache[i] = val;
            }
        }
        cache_size = needed;
    }
    return cache[n];
}
