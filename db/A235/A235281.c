/*
 * A235281  Number of (n+1) X (n+1) 0..3 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 4
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A235281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            a(n-1)+9*a(n-2)-9*a(n-3)-26*a(n-4)+26*a(n-5)+24*a(n-6)-24*a(n-7)
 *            a(1)=20, a(2)=68, a(3)=148, a(4)=380, a(5)=740, a(6)=1724,
 *            a(7)=3220
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of allocated elements     */

    if (n < 1)
        return 0;   /* below offset – behaviour unspecified */

    /* Extend cache if necessary */
    if (cache_size <= (size_t)n)
    {
        size_t new_size = (size_t)n + 1; /* allow index n */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) /* allocation failure – unlikely during tests */
        {
            cache = NULL;
            cache_size = 0;
            return 0;
        }
        cache = tmp;

        static const seq_t init[8] = {0, 20, 68, 148, 380, 740, 1724, 3220};

        for (size_t i = cache_size; i <= n; ++i)
        {
            if (i == 0) /* unused slot */
                continue;

            if (i <= 7)          /* base values */
                cache[i] = init[i];
            else                 /* recurrence for i >= 8 */
            {
                seq_t val = cache[i - 1];
                val += (seq_t)9  * cache[i - 2];
                val -= (seq_t)9  * cache[i - 3];
                val -= (seq_t)26 * cache[i - 4];
                val += (seq_t)26 * cache[i - 5];
                val += (seq_t)24 * cache[i - 6];
                val -= (seq_t)24 * cache[i - 7];
                cache[i] = val;
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
