/*
 * A026567  a(n) = Sum_{i=0..2*n} Sum_{j=0..i} T(i, j), where T is given by
 *          A026552.
 * https://oeis.org/A026567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6 * a(n-2) + 7, with a(0) = 1, a(1) = 4
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 48, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cache to memoise computed terms */
static seq_t *cache   = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

/* ensure the cache has room for indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap <= n)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache_len == 0) {
        ensure_cache(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache_len = 2;
    }

    /* compute any missing entries up to n */
    if (n >= cache_len) {
        ensure_cache(n);
        for (idx_t i = cache_len; i <= n; ++i) {
            /* recurrence only applies from i >= 2 */
            if (i >= 2) {
                seq_t val = cache[i - 2] * (seq_t)6 + (seq_t)7;
                cache[i] = val;
            }
        }
        cache_len = n + 1;
    }

    return cache[n];
}
