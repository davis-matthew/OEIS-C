/*
 * A195852  Column 8 of array A195825. Also column 1 of triangle A195842. Also
 *          1 together with the row sums of triangle A195842.
 * https://oeis.org/A195852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts congruent to * 0, 1
 *            or 9 (mod 10). This is computed by dynamic programming * with
 *            memoisation for random access to any n
 * Range:     u8 n <= 58, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static cache for memoisation */
static seq_t *cache = NULL;       /* array of partition counts */
static idx_t  cache_len = 0;     /* number of elements in cache (cache_len-1 is max n computed) */

/* ensure the cache has at least n+1 elements */
static void grow_cache(idx_t n)
{
    if (n < cache_len) return;
    size_t new_cap = cache_len ? cache_len * 2 : 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_len = new_cap;
}

/* compute all values up to n inclusive, storing them in cache */
static void compute_upto(idx_t n)
{
    grow_cache(n);
    /* initialise */
    for (idx_t i = 0; i <= n; ++i) cache[i] = 0;
    cache[0] = 1;       /* there is one way to partition zero */

    /* iterate over allowed part sizes */
    for (idx_t k = 1; ; ++k) {
        idx_t p1 = 10 * k;      /* 10, 20, 30, ...   */
        idx_t p2 = 10 * k - 1;  /* 9, 19, 29, ...    */
        idx_t p3 = 10 * k - 9;  /* 1, 11, 21, ...    */

        bool any = false;
        if (p1 <= n) { any = true; for (idx_t i = p1; i <= n; ++i) cache[i] += cache[i - p1]; }
        if (p2 <= n) { any = true; for (idx_t i = p2; i <= n; ++i) cache[i] += cache[i - p2]; }
        if (p3 <= n) { any = true; for (idx_t i = p3; i <= n; ++i) cache[i] += cache[i - p3]; }

        if (!any) break;    /* no more parts within the bound */
    }
}

seq_t a(idx_t n)
{
    compute_upto(n);
    return cache[n];
}
