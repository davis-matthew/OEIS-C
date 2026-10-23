/*
 * A157089  Consider all consecutive integer Pythagorean septuples (X, X+1,
 *          X+2, X+3, Z-2, Z-1, Z) ordered by increasing Z; sequence gives Z
 *          values.
 * https://oeis.org/A157089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=3, a(1)=27; for n>1 a(n)=14*a(n-1)-a(n-2)-12
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
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

    /* initialise cache with first two values */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0ULL;   /* fallback, should not happen */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)27;
    }

    /* extend cache up to requested index */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0ULL;   /* fallback */
        cache = tmp;
        for (size_t i = cache_size; i < new_size; ++i) {
            if (i == 0)
                cache[i] = (seq_t)3;
            else if (i == 1)
                cache[i] = (seq_t)27;
            else
                cache[i] = (seq_t)14 * cache[i-1] - cache[i-2] - (seq_t)12;
        }
        cache_size = new_size;
    }

    return cache[n];
}
