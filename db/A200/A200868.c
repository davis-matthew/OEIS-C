/*
 * A200868  Number of 0..5 arrays x(0..n+1) of n+2 elements without any
 *          interior element greater than both neighbors or less than both
 *          neighbors.
 * https://oeis.org/A200868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-3) + 7*a(n-4) + 3*a(n-5) + 2*a(n-6) +
 *            3*a(n-7) + a(n-8) for n >= 9, with initial values: a(1)=106,
 *            a(2)=356, a(3)=1168, a(4)=3886, a(5)=12890, a(6)=42744,
 *            a(7)=141688, a(8)=469726
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 15, u64 n <= 34, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* LRU: static cache holding values from 1..cache_n */
    static seq_t *cache  = NULL;
    static idx_t cache_n = 0;  /* largest n for which cache[n] is valid */

    if (n < 1) return (seq_t)0;   /* behaviour below offset is irrelevant */

    /* Expand cache if necessary */
    if (n > cache_n)
    {
        /* allocate or grow cache */
        if (cache == NULL)
        {
            cache = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
            if (cache == NULL) abort();           /* out of memory */
            cache[0] = (seq_t)0;                  /* unused slot */
            cache_n = 0;
        }
        else if (n > cache_n)
        {
            seq_t *tmp = (seq_t*)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
            if (tmp == NULL) abort();           /* out of memory */
            cache = tmp;
        }

        /* fill new entries */
        for (idx_t i = cache_n + 1; i <= n; ++i)
        {
            if (i == 1)       cache[i] = (seq_t)106;
            else if (i == 2)  cache[i] = (seq_t)356;
            else if (i == 3)  cache[i] = (seq_t)1168;
            else if (i == 4)  cache[i] = (seq_t)3886;
            else if (i == 5)  cache[i] = (seq_t)12890;
            else if (i == 6)  cache[i] = (seq_t)42744;
            else if (i == 7)  cache[i] = (seq_t)141688;
            else if (i == 8)  cache[i] = (seq_t)469726;
            else
            {
                /* recurrence for i >= 9 */
                cache[i] =  3*cache[i-1]
                          + cache[i-3]
                          + 7*cache[i-4]
                          + 3*cache[i-5]
                          + 2*cache[i-6]
                          + 3*cache[i-7]
                          + cache[i-8];
            }
        }
        cache_n = n;
    }

    return cache[n];
}
