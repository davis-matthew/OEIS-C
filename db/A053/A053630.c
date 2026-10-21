/*
 * A053630  Pythagorean spiral: a(n-1), a(n)-1 and a(n) are sides of a right
 *          triangle.
 * https://oeis.org/A053630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (a(n-1)^2 + 1)/2, a(1) = 3
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoisation cache: cache[i] holds a(i+1) (0‑based). */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

seq_t a(idx_t n)
{
    /* Undefined for n < 1; return 0 as a safe sentinel. */
    if (n < 1)
        return (seq_t)0;

    /* Expand the cache to at least n elements. */
    if (cache_len < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        if (new_cap < (size_t)n)
            new_cap = n;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;

        /* Compute all new terms up to n. */
        for (size_t i = cache_len; i < (size_t)n; ++i) {
            seq_t val;
            if (i == 0)            /* a(1) */
                val = (seq_t)3;
            else {
                seq_t prev = cache[i - 1];
                val = (prev * prev + (seq_t)1) / (seq_t)2;
            }
            cache[i] = val;
        }
        cache_len = (size_t)n;
    }

    return cache[n - 1];
}
