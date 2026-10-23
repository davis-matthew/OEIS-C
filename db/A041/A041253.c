/*
 * A041253  Denominators of continued fraction convergents to sqrt(138).
 * https://oeis.org/A041253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=3, a(3)=4, a(4)=91, a(5)=95, a(6)=281,
 *            a(7)=376, a(n) = 94*a(n-4) - a(n-8) for n ≥ 8
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization (global, not thread‑local) */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;        /* number of elements cached */

    /* compute index as size_t for array access */
    size_t idx = (size_t)n;

    /* if cache too small, enlarge and fill missing terms */
    if (idx + 1 > cache_size)
    {
        size_t new_size = idx + 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            abort();                       /* abort on allocation failure */

        cache = tmp;

        /* compute new entries */
        for (size_t i = cache_size; i < new_size; ++i)
        {
            if (i == 0)         cache[i] = 1;
            else if (i == 1)    cache[i] = 1;
            else if (i == 2)    cache[i] = 3;
            else if (i == 3)    cache[i] = 4;
            else if (i == 4)    cache[i] = 91;
            else if (i == 5)    cache[i] = 95;
            else if (i == 6)    cache[i] = 281;
            else if (i == 7)    cache[i] = 376;
            else                cache[i] = (seq_t)94 * cache[i-4] - cache[i-8];
        }

        cache_size = new_size;
    }

    return cache[idx];
}
