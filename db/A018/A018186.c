/*
 * A018186  a(n) = 3*a(n-2) + a(n-4), with a(0) = 1, a(1) = 3, a(2) = 6, and
 *          a(3) = 9.
 * https://oeis.org/A018186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2) + a(n-4), a(0)=1, a(1)=3, a(2)=6, a(3)=9
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 36, u64 n <= 73, u128 n <= 147,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_realloc.h"


seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t last_computed = 3;   /* highest index already computed */

    if (n + 1 > cache_size)
    {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = seq_realloc(cache, (size_t)cache_size, new_size);
        if (!tmp)
            return (seq_t)0;   /* allocation failed */
        cache = tmp;
        cache_size = new_size;

        /* initialise the first four terms once the array is large enough */
        if (cache_size > 0) cache[0] = (seq_t)1;
        if (cache_size > 1) cache[1] = (seq_t)3;
        if (cache_size > 2) cache[2] = (seq_t)6;
        if (cache_size > 3) cache[3] = (seq_t)9;
    }

    if (n < 4)
        return cache[n];

    idx_t start = last_computed + 1;
    if (start < 4)
        start = 4;

    for (idx_t i = start; i <= n; ++i)
    {
        cache[i] = (cache[i - 2] * (seq_t)3) + cache[i - 4];
    }
    last_computed = n;

    return cache[n];
}
