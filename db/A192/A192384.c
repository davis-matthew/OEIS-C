/*
 * A192384  Coefficient of x in the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 8*a(n-2) - 4*a(n-3) - 4*a(n-4) with initial
 *            values a(1)=0, a(2)=2, a(3)=4, a(4)=24
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                         /* undefined for n<1, return 0 */

    static seq_t *cache = NULL;          /* cache[0] unused */
    static idx_t cache_size = 0;

    if (n > cache_size) {
        /* grow cache to hold up to n */
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)
            abort();                     /* allocation failure */
        cache = tmp;

        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i == 1)
                cache[i] = (seq_t)0;
            else if (i == 2)
                cache[i] = (seq_t)2;
            else if (i == 3)
                cache[i] = (seq_t)4;
            else if (i == 4)
                cache[i] = (seq_t)24;
            else
                cache[i] =
                    (seq_t)2 * cache[i - 1]
                  + (seq_t)8 * cache[i - 2]
                  - (seq_t)4 * cache[i - 3]
                  - (seq_t)4 * cache[i - 4];
        }
        cache_size = n;
    }

    return cache[n];
}
