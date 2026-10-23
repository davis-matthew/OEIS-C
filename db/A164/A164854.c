/*
 * A164854  Diagonal sum of generalized Pascal triangle; (10^n,1).
 * https://oeis.org/A164854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+11*a(n-2)-10*a(n-3)-10*a(n-4),
 *            a(0)=1,a(1)=1,a(2)=11,a(3)=12
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* capacity of the array */
    static size_t cache_len = 0;   /* number of elements already computed */

    /* enlarge cache if necessary */
    if ((size_t)n >= cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 32;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i)
    {
        if (i == 0)          cache[i] = (seq_t)1;
        else if (i == 1)     cache[i] = (seq_t)1;
        else if (i == 2)     cache[i] = (seq_t)11;
        else if (i == 3)     cache[i] = (seq_t)12;
        else
        {
            seq_t t = cache[i-1];
            t += cache[i-2] * (seq_t)11;
            t -= cache[i-3] * (seq_t)10;
            t -= cache[i-4] * (seq_t)10;
            cache[i] = t;
        }
    }
    cache_len = (size_t)n + 1;

    return cache[n];
}
