/*
 * A169079  Number of reduced words of length n in Coxeter group on 18
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^25 = I.
 * https://oeis.org/A169079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for 1 ≤ n ≤ 24: a(n)=16*∑_{i=0}^{n-1} a(i)+2; for n ≥
 *            25: a(n)=16*∑_{i=n-24}^{n-1} a(i)-136*a(n-25);
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements currently stored: 0..cache_size-1 */

    /* ensure cache is large enough up to index n */
    while (cache_size <= n)
    {
        /* reallocate to accommodate one more element */
        seq_t *new_cache = (seq_t *)realloc(cache, (cache_size + 1) * sizeof(seq_t));
        if (!new_cache)
        {
            /* allocation failed – abort */
            abort();
        }
        cache = new_cache;

        idx_t i = cache_size;

        if (i == 0)
        {
            cache[0] = (seq_t)1;
        }
        else if (i <= 24)
        {
            /* sum of all previous terms a[0]..a[i-1] */
            seq_t sum = 0;
            for (idx_t j = 0; j < i; ++j)
                sum += cache[j];
            cache[i] = 16 * sum + (seq_t)2;
        }
        else
        {
            /* sum of the previous 24 terms */
            seq_t sum = 0;
            for (idx_t j = i - 24; j < i; ++j)
                sum += cache[j];
            cache[i] = 16 * sum - 136 * cache[i - 25];
        }

        ++cache_size;
    }

    return cache[n];
}
