/*
 * A052734  a(n) = 4^(n-1) * n! * Catalan(n-1) for n > 0, with a(0) = 0.
 * https://oeis.org/A052734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=0, a(1)=1, and for n≥2 * a(n) = 8 * (2*(n-1)-1) * a(n-1)
 *            (derived from 8*(1-2*(n-1))*a(n-1)+a(n)=0)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes A052734 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size)
    {
        /* enlarge cache to hold indices up to n */
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL)    /* allocation failed; abort */
            abort();
        cache = new_cache;

        /* initialise newly added entries */
        for (idx_t i = cache_size; i <= n; ++i)
        {
            if (i == 0)
                cache[0] = (seq_t)0;
            else if (i == 1)
                cache[1] = (seq_t)1;
            else
            {
                /* compute a(i) from a(i-1) */
                seq_t factor = (seq_t)8 * ((seq_t)(2 * (i - 1) - 1));
                cache[i] = factor * cache[i - 1];
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
