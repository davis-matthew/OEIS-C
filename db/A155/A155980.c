/*
 * A155980  First differences of A135351.
 * https://oeis.org/A155980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) for n >= 4; a(0) = 0, a(1) = 2, a(2) =
 *            -2, a(3) = 3
 * Range:     i8 n <= 10, i16 n <= 18, i32 n <= 34, i64 n <= 66, i128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    if (cache_size <= idx) {
        size_t new_size = idx + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) exit(1);
        cache = new_cache;
        for (size_t i = cache_size; i < new_size; ++i) {
            if (i == 0)            cache[i] = 0;
            else if (i == 1)       cache[i] = 2;
            else if (i == 2)       cache[i] = (seq_t)(-2);
            else if (i == 3)       cache[i] = 3;
            else                   cache[i] = cache[i-1] + cache[i-2] * ((seq_t)2);
        }
        cache_size = new_size;
    }
    return cache[idx];
}
