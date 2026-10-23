/*
 * A041014  Numerators of continued fraction convergents to sqrt(11).
 * https://oeis.org/A041014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 20*a(n-2) - a(n-4), a(0)=3, a(1)=10, a(2)=63, a(3)=199
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Ensure the cache can hold index n */
    if (n + 1 > cache_size)
    {
        idx_t old_size = cache_size;
        idx_t new_cap  = (n + 1 > 4) ? (n + 1) : 4;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failed */
        cache = tmp;
        cache_size = new_cap;

        /* Initialize the first four terms */
        if (cache_size >= 1) cache[0] = (seq_t)3;
        if (cache_size >= 2) cache[1] = (seq_t)10;
        if (cache_size >= 3) cache[2] = (seq_t)63;
        if (cache_size >= 4) cache[3] = (seq_t)199;

        /* Compute missing terms */
        idx_t start = (old_size < 4) ? 4 : old_size;
        for (idx_t i = start; i <= n; ++i)
        {
            cache[i] = ((seq_t)20) * cache[i - 2] - cache[i - 4];
        }
    }

    return cache[n];
}
