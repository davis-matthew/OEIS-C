/*
 * A008845  Numbers k such that k+1 and k/2+1 are squares.
 * https://oeis.org/A008845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 34*a(n-1) - a(n-2) + 48 Initial values: a(0) = 0, a(1) =
 *            48 Sequence A008845 – numbers k such that k+1 and k/2+1 are
 *            squares
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoization cache */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* capacity of cache */
    static idx_t  cache_len = 0;   /* number of elements currently stored */

    /* Grow cache as necessary */
    if (cache_len <= n)
    {
        size_t needed = (size_t)(n + 1);
        if (cache_cap < needed)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Initialise first term */
        if (cache_len == 0)
        {
            cache[0] = 0;
            cache_len = 1;
        }

        /* Initialise second term if needed */
        if (n >= 1 && cache_len == 1)
        {
            cache[1] = 48;
            cache_len = 2;
        }

        /* Compute remaining terms up to n */
        while (cache_len <= n)
        {
            idx_t k = cache_len;
            seq_t a_k1 = cache[(size_t)k - 1];
            seq_t a_k2 = (k >= 2) ? cache[(size_t)k - 2] : (seq_t)0;
            /* recurrence: a(k) = 34*a(k-1) - a(k-2) + 48 */
            seq_t val = 34 * a_k1 - a_k2 + 48;
            cache[(size_t)k] = val;
            ++cache_len;
        }
    }
    return cache[(size_t)n];
}
