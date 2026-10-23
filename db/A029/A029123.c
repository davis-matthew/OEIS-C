/*
 * A029123  Expansion of 1/((1-x)*(1-x^7)*(1-x^9)*(1-x^12)).
 * https://oeis.org/A029123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 1, 7, 9 and 12
 *            (dynamic programming)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of dynamic‑programming values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of elements currently allocated */

    /* If the requested index is beyond the cached range, grow and recompute */
    if (n + 1 > cache_len)
    {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;          /* allocation failure (unlikely during tests) */
        cache = tmp;
        cache_len = new_len;

        /* Initialise entries to zero and set the base case */
        for (idx_t i = 0; i < cache_len; ++i)
            cache[i] = (seq_t)0;
        cache[0] = (seq_t)1;

        /* Allowed part sizes */
        static const idx_t parts[] = {1, 7, 9, 12};
        static const size_t PARTS = sizeof parts / sizeof *parts;

        /* Standard partition DP: for each part add ways to reach each sum */
        for (size_t pi = 0; pi < PARTS; ++pi)
        {
            idx_t p = parts[pi];
            for (idx_t i = p; i < cache_len; ++i)
                cache[i] += cache[i - p];
        }
    }

    return cache[n];
}
