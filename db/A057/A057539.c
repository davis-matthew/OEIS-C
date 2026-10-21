/*
 * A057539  Birthday set of order 7, i.e., numbers congruent to +- 1 modulo 2,
 *          3, 4, 5, 6 and 7.
 * https://oeis.org/A057539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-8) + 210 for n > 8 initial values: a(1)=1, a(2)=29,
 *            a(3)=41, a(4)=71, a(5)=139, a(6)=169, a(7)=181, a(8)=209
 * Range:     u8 n <= 11, u16 n <= 2497, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The cached values are kept in a static array that grows
   automatically to the greatest requested index.  This allows
   unrelated calls to any n, in any order, while keeping the
   overall complexity linear in the maximum n seen. */

#include <stddef.h>
#include <stdlib.h>

/* seq_t, idx_t are provided by the build environment. */
seq_t a(idx_t n)
{
    /* For n == 0 (outside the offset) the behaviour is undefined
       – return 0 to avoid accessing invalid memory. */
    if (n == 0U)
        return (seq_t)0;

    /* Static cache table */
    static seq_t *cache = NULL;      /* array of computed terms */
    static idx_t  cache_size = 0;   /* number of elements stored */
    static idx_t  cache_cap  = 0;   /* capacity of the array */

    /* Ensure the cache can hold n terms */
    if (n > cache_size)
    {
        /* Grow the cache if necessary */
        if (n > cache_cap)
        {
            idx_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
            if (new_cap < n)
                new_cap = n;
            seq_t *tmp = (cache_cap == 0) ? (seq_t *)malloc(new_cap * sizeof(seq_t))
                                           : (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL)  /* allocation failure – fall back to minimal result */
                return (seq_t)0;
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute missing entries up to n */
        for (idx_t i = cache_size + 1; i <= n; ++i)
        {
            /* index in the array is i-1 */
            if (i <= 8U)
            {
                /* Initialise the first eight terms explicitly */
                switch (i)
                {
                    case 1U: cache[i - 1] = (seq_t)1;   break;
                    case 2U: cache[i - 1] = (seq_t)29;  break;
                    case 3U: cache[i - 1] = (seq_t)41;  break;
                    case 4U: cache[i - 1] = (seq_t)71;  break;
                    case 5U: cache[i - 1] = (seq_t)139; break;
                    case 6U: cache[i - 1] = (seq_t)169; break;
                    case 7U: cache[i - 1] = (seq_t)181; break;
                    case 8U: cache[i - 1] = (seq_t)209; break;
                }
            }
            else
            {
                /* Recurrence: a(n) = a(n-8) + 210 */
                cache[i - 1] = cache[(i - 8) - 1] + (seq_t)210;
            }
        }
        cache_size = n;
    }

    return cache[n - 1];
}
