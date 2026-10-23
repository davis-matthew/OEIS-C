/*
 * A027390  Number of labeled servers of dimension 3.
 * https://oeis.org/A027390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 3*(n-1)*a(n-2) + (n-1)*(n-2)*a(n-3) initial
 *            values: a(0)=1, a(1)=3, a(2)=12
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 21, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache state */
    static seq_t *cache = NULL;    /* array for stored values */
    static size_t cache_cap = 0;   /* capacity of the array */
    static size_t cache_filled = 0;/* number of values stored */

    /* ensure array has capacity for indices up to n */
    if (n >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 1 : cache_cap;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (cache && cache_filled > 0)
            memcpy(new_cache, cache, cache_filled * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* initialize base values if necessary */
    if (cache_filled <= 0 && n >= 0)
        cache[0] = (seq_t)1;
    if (cache_filled <= 1 && n >= 1)
        cache[1] = (seq_t)3;
    if (cache_filled <= 2 && n >= 2)
        cache[2] = (seq_t)12;
    if (cache_filled < 3)
        cache_filled = 3;

    /* compute values up to n */
    for (size_t i = cache_filled; i <= (size_t)n; ++i) {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)3;
        else if (i == 2)
            cache[i] = (seq_t)12;
        else
        {
            seq_t im1 = (seq_t)(i - 1);
            seq_t im2 = (seq_t)(i - 2);
            seq_t term1 = (seq_t)3 * cache[i-1];
            seq_t term2 = (seq_t)3 * im1 * cache[i-2];
            seq_t term3 = im1 * im2 * cache[i-3];
            cache[i] = term1 + term2 + term3;
        }
    }
    cache_filled = (size_t)n + 1;

    return cache[(size_t)n];
}
