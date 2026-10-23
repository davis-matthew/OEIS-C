/*
 * A023442  Dying rabbits: a(n) = a(n-1) + a(n-2) - a(n-12).
 * https://oeis.org/A023442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-12) a(0) = 0, a(1) = 1, a(2) = 1,
 *            a(3) = 2, a(4) = 3, a(5) = 5, a(6) = 8, a(7) = 13, a(8) = 21,
 *            a(9) = 34, a(10) = 55, a(11) = 89, a(12) = 144
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* dynamically growing cache of computed terms */
    static size_t cached = 0;     /* number of elements currently cached */

    /* Ensure that the cache is large enough for index n */
    size_t idx = (size_t)n;
    if (idx >= cached)
    {
        size_t new_size = idx + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;   /* out of memory; return 0 as a fallback */

        /* Compute terms from the current cached size up to the requested index */
        for (size_t i = cached; i < new_size; ++i)
        {
            if (i == 0)
            {
                cache[i] = 0;
            }
            else if (i == 1)
            {
                cache[i] = 1;
            }
            else
            {
                seq_t val = cache[i - 1] + cache[i - 2];
                if (i >= 12)
                    val -= cache[i - 12];
                cache[i] = val;
            }
        }
        cached = new_size;
    }

    return cache[idx];
}
