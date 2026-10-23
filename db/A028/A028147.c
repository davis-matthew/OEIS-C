/*
 * A028147  Expansion of 1/((1-4x)(1-7x)(1-8x)(1-11x)).
 * https://oeis.org/A028147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 30*a(n-1)-325*a(n-2)+1500*a(n-3)-2464*a(n-4) Initial
 *            values: a(0)=1, a(1)=30, a(2)=575, a(3)=9000
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* A small cache that keeps all values computed so far.
       The cache grows only when a larger index is requested. */
    static seq_t *cache = NULL;
    static idx_t    cache_len = 0;   /* length of the currently allocated cache */

    /* Extend cache if necessary */
    if (cache_len <= n)
    {
        /* Allocate space for indices 0..n inclusive */
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL)                /* allocation failed – return 0 as a safe fallback */
            return (seq_t)0;
        cache = tmp;

        /* Compute any new terms */
        for (idx_t i = cache_len; i <= n; ++i)
        {
            if (i == 0)
                cache[0] = (seq_t)1;
            else if (i == 1)
                cache[1] = (seq_t)30;
            else if (i == 2)
                cache[2] = (seq_t)575;
            else if (i == 3)
                cache[3] = (seq_t)9000;
            else
            {
                /* Recurrence: a(n)=30*a(n-1)-325*a(n-2)+1500*a(n-3)-2464*a(n-4) */
                seq_t term1 = cache[i - 1] * (seq_t)30;
                seq_t term2 = cache[i - 2] * (seq_t)325;
                seq_t term3 = cache[i - 3] * (seq_t)1500;
                seq_t term4 = cache[i - 4] * (seq_t)2464;
                cache[i] = (term1 - term2) + (term3 - term4);
            }
        }
        cache_len = (idx_t)new_len;
    }

    return cache[n];
}
