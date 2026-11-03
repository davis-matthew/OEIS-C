/*
 * A072638  Number of unary-binary rooted trees of height at most n.
 * https://oeis.org/A072638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (a(n-1)+1)*(a(n-1)+2)/2, a(0) = 0
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 10 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Lazy memoisation of computed terms. */
    static seq_t *cache = NULL;   /* array of already computed values */
    static idx_t cache_len = 0;   /* current length of the array */

    /* Allocate initial cache for the first term if needed. */
    if (cache_len == 0)
    {
        cache_len = 1;
        cache = mem_new(seq_t, cache_len);
        if (!cache) return (seq_t)0;       /* memory failure, return zero */
        cache[0] = 0;                      /* a(0) = 0 */
    }

    /* Extend cache up to n if needed. */
    if (n + 1 > cache_len)
    {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = mem_resize(seq_t, cache, new_len);
        if (!tmp) return (seq_t)0;         /* memory failure, return zero */
        cache = tmp;

        /* Compute missing terms iteratively. */
        for (idx_t i = cache_len; i < new_len; ++i)
        {
            seq_t prev = cache[i - 1];
            seq_t t1   = prev + ((seq_t)1);
            seq_t t2   = prev + ((seq_t)2);
            seq_t prod = t1 * t2;
            cache[i]   = prod / ((seq_t)2);
        }

        cache_len = new_len;
    }

    return cache[n];
}
