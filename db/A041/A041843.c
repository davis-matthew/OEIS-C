/*
 * A041843  Denominators of continued fraction convergents to sqrt(443).
 * https://oeis.org/A041843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, less
 * Formula:   a(n) = 884*a(n-2) - a(n-4) (n ≥ 4) with a(0) = 1, a(1) = 21,
 *            a(2) = 883, a(3) = 18564
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)
            exit(1);                      /* allocation failure */

        for (size_t i = cache_size; i < new_size; ++i) {
            if (i == 0)
                new_cache[i] = (seq_t)1;
            else if (i == 1)
                new_cache[i] = (seq_t)21;
            else if (i == 2)
                new_cache[i] = (seq_t)883;
            else if (i == 3)
                new_cache[i] = (seq_t)18564;
            else
                new_cache[i] = (seq_t)884 * new_cache[i-2] - new_cache[i-4];
        }

        cache = new_cache;
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
