/*
 * A016091  Expansion of 1/((1-8x)(1-9x)(1-10x)(1-12x)).
 * https://oeis.org/A016091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 39*a(n-1) - 566*a(n-2) + 3624*a(n-3) - 8640*a(n-4)
 *            Initial values: a(0)=1, a(1)=39, a(2)=955, a(3)=18795
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms; indices 0..cache_size-1 are stored. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    size_t req = (size_t)n;

    if (req >= cache_size)
    {
        size_t new_size = req + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) /* allocation failed: return zero */
            return (seq_t)0;
        cache = tmp;

        for (size_t i = cache_size; i < new_size; ++i)
        {
            if (i == 0)          cache[i] = (seq_t)1;
            else if (i == 1)     cache[i] = (seq_t)39;
            else if (i == 2)     cache[i] = (seq_t)955;
            else if (i == 3)     cache[i] = (seq_t)18795;
            else
            {
                seq_t a1 = cache[i-1];
                seq_t a2 = cache[i-2];
                seq_t a3 = cache[i-3];
                seq_t a4 = cache[i-4];
                cache[i] = (seq_t)39 * a1
                          - (seq_t)566 * a2
                          + (seq_t)3624 * a3
                          - (seq_t)8640 * a4;
            }
        }
        cache_size = new_size;
    }

    return cache[req];
}
