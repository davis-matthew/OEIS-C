/*
 * A015520  a(n) = 2*a(n-1) + 11*a(n-2), a(0) = 0, a(1) = 1.
 * https://oeis.org/A015520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 11*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cache of previously computed terms */
    static idx_t cache_len = 0;     /* number of terms currently stored (max index + 1) */

    if (n >= cache_len)               /* need to extend the cache */
    {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) /* out of memory: terminate */
            exit(1);
        cache = tmp;

        /* initialise the first terms if they are being added for the first time */
        if (cache_len == 0)
        {
            cache[0] = 0;
            if (new_len > 1)
                cache[1] = 1;
        }
        else if (cache_len == 1)      /* only a(0) existed before */
        {
            cache[1] = 1;
        }
        /* compute new terms from the highest already computed index up to n */
        idx_t start = cache_len;
        if (start < 2)
            start = 2;
        for (idx_t i = start; i < new_len; ++i)
        {
            cache[i] = ((seq_t)2 * cache[i - 1]) + ((seq_t)11 * cache[i - 2]);
        }
        cache_len = new_len;
    }
    return cache[n];
}
