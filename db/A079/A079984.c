/*
 * A079984  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=3, r=3, I={-1,1,2}.
 * https://oeis.org/A079984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            a(n-1)+a(n-2)+a(n-3)-a(n-4)+a(n-5)+3*a(n-6)-4*a(n-7)-4*a(n-8)-2*a(n-9)
 *            +4*a(n-10)+2*a(n-11)-4*a(n-12)+4*a(n-13)+3*a(n-14)-a(n-15)-a(n-16)-a(n-17)
 *            +a(n-18)-a(n-19)-a(n-20) with initial values a(0)=1, a(1)=1,
 *            a(2)=1, a(3)=1, a(4)=2, a(5)=5, a(6)=10, a(7)=16, a(8)=26,
 *            a(9)=43, a(10)=80, a(11)...
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* the function follows the requirements of the build environment */
seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_size = 0;

    /* ensure the cache can hold n+1 elements */
    if (cache_size <= n)
    {
        size_t needed = (size_t)n + 1;
        if (cache_cap < needed)
        {
            size_t new_cap = (cache_cap > 0) ? cache_cap : 64;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* initial values for indices 0 .. 19 */
        static const seq_t init[20] = {
            1, 1, 1, 1, 2, 5, 10, 16, 26, 43,
            80, 148, 264, 465, 816, 1444, 2588,
            4619, 8214, 14591
        };

        size_t start = cache_size;
        if (start < 20)
        {
            size_t end = (size_t)20;
            if (end > needed) end = needed;
            for (size_t i = start; i < end; ++i)
                cache[i] = init[i];
            cache_size = end;
        }

        /* compute remaining terms using the linear recurrence for n >= 20 */
        for (size_t i = cache_size; i < needed; ++i)
        {
            /* all indices i-1 .. i-20 exist because i >= 20 */
            seq_t sum = cache[i-1]            + cache[i-2]
                        + cache[i-3]          - cache[i-4]
                        + cache[i-5] + 3*cache[i-6]
                        - 4*cache[i-7] - 4*cache[i-8] - 2*cache[i-9]
                        + 4*cache[i-10] + 2*cache[i-11]
                        - 4*cache[i-12] + 4*cache[i-13] + 3*cache[i-14]
                        - cache[i-15] - cache[i-16] - cache[i-17]
                        + cache[i-18] - cache[i-19] - cache[i-20];
            cache[i] = sum;
        }
        cache_size = needed;
    }

    return cache[(size_t)n];
}
