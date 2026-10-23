/*
 * A163453  Number of reduced words of length n in Coxeter group on 19
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^5 = I.
 * https://oeis.org/A163453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=19, a(2)=342, a(3)=6156, a(4)=110808, a(5)=1994373
 *            For n>=6: a(n)=17(a(n-1)+a(n-2)+a(n-3)+a(n-4)) - 153 a(n-5)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    size_t idx = (size_t)n;

    if (idx < cache_size)
        return cache[idx];

    size_t new_size = idx + 1;
    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_cache)
        return (seq_t)0; /* allocation failed */

    /* copy previously computed values */
    for (size_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];

    /* free old buffer */
    if (cache)
        free(cache);

    cache = new_cache;
    size_t old_size = cache_size;
    cache_size = new_size;

    for (size_t i = old_size; i <= idx; ++i)
    {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)19;
        else if (i == 2)
            cache[i] = (seq_t)342;
        else if (i == 3)
            cache[i] = (seq_t)6156;
        else if (i == 4)
            cache[i] = (seq_t)110808;
        else if (i == 5)
            cache[i] = (seq_t)1994373;
        else
        {
            seq_t sum = cache[i - 1] + cache[i - 2] + cache[i - 3] + cache[i - 4];
            cache[i] = ((seq_t)17 * sum) - ((seq_t)153 * cache[i - 5]);
        }
    }

    return cache[idx];
}
