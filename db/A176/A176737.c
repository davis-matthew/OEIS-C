/*
 * A176737  Expansion of 1 / (1-4*x^2-3*x^3). (4,3)-Padovan sequence.
 * https://oeis.org/A176737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=0; for n>=2 a(n)=a(n-1)+3*a(n-2)+(-1)^n
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 54, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    if (n + 1 > cache_size)
    {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)
            abort(); /* memory allocation failure */

        cache = new_cache;

        for (idx_t i = cache_size; i < new_size; ++i)
        {
            if (i == 0)
                cache[i] = (seq_t)1;
            else if (i == 1)
                cache[i] = (seq_t)0;
            else
            {
                seq_t temp = cache[i - 1] + (seq_t)3 * cache[i - 2];
                if ((i & 1ull) == 0)
                    cache[i] = temp + (seq_t)1;
                else
                    cache[i] = temp - (seq_t)1;
            }
        }

        cache_size = new_size;
    }

    return cache[n];
}
