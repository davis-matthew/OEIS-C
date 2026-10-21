/*
 * A062113  a(0)=1; a(1)=2; a(n) = a(n-1) + a(n-2)*(3 - (-1)^n)/2.
 * https://oeis.org/A062113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 36, u64 n <= 72, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; a(1)=2; a(n)=a(n-1)+a(n-2)*((3-(-1)^n)/2)
   (i.e. multiply a(n‑2) by 1 for even n and by 2 for odd n) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache holds all computed values up to the largest index seen so far */
    static seq_t *cache      = NULL;
    static idx_t cache_len   = 0;

    /* grow cache if necessary */
    if (n + 1 > cache_len)
    {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();          /* failure to allocate – terminate */
        cache = tmp;

        /* compute all missing terms */
        for (idx_t i = cache_len; i < new_len; ++i)
        {
            if (i == 0)
                cache[i] = (seq_t)1;
            else if (i == 1)
                cache[i] = (seq_t)2;
            else
            {
                /* coefficient: 1 for even n, 2 for odd n */
                seq_t coef = ((i & (idx_t)1) == 0) ? (seq_t)1 : (seq_t)2;
                cache[i] = cache[i-1] + cache[i-2] * coef;
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
