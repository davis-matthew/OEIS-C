/*
 * A028118  Expansion of 1/((1-4x)(1-5x)(1-7x)(1-11x)).
 * https://oeis.org/A028118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 27*a(n-1) - 259*a(n-2) + 1053*a(n-3) - 1540*a(n-4) a(0) =
 *            1, a(1) = 27, a(2) = 470, a(3) = 6750 (Derived from
 *            1/((1-4x)(1-5x)(1-7x)(1-11x)) )
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap   = 0;    /* allocated size */
    static size_t len   = 0;    /* number of elements stored (valid 0..len-1) */

    /* grow the cache if needed */
    if ((size_t)n >= len)
    {
        size_t new_cap = (cap == 0) ? 16 : cap * 2;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();   /* allocation failure */
        cache = new_cache;
        cap   = new_cap;
    }

    /* fill missing terms up to n */
    for (size_t i = len; i <= (size_t)n; ++i)
    {
        if (i == 0)      cache[i] = (seq_t)1;
        else if (i == 1) cache[i] = (seq_t)27;
        else if (i == 2) cache[i] = (seq_t)470;
        else if (i == 3) cache[i] = (seq_t)6750;
        else
        {
            /* recurrence using only +, -, *, */
            cache[i] = (seq_t)27 * cache[i-1]
                     - (seq_t)259 * cache[i-2]
                     + (seq_t)1053 * cache[i-3]
                     - (seq_t)1540 * cache[i-4];
        }
    }
    len = (size_t)n + 1;   /* cache contains up to n */

    return cache[(size_t)n];
}
