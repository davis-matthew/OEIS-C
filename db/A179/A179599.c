/*
 * A179599  Eight white kings and one red king on a 3 X 3 chessboard. G.f.: (1
 *          + 4*x)/(1 - 3*x - 8*x^2).
 * https://oeis.org/A179599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 3*a(n-1) + 8*a(n-2)
 * with a(0) = 1, a(1) = 7
 *
 * Computed with memoization in O(n) time per new maximum n.
 */

#include <stdlib.h>
#include <stddef.h>

/* Static cache for memoization */
static seq_t *cache = NULL;        /* array of computed terms */
static idx_t cache_cap = 0;        /* capacity of the array */
static idx_t cache_used = 0;       /* number of terms actually stored */

/* Ensure the cache can hold at least n+1 terms */
static void ensure_cache_cap(idx_t n)
{
    if (n < cache_cap)
        return;

    /* Grow capacity exponentially to keep amortized O(1) per extend */
    idx_t new_cap = cache_cap == 0 ? 4 : cache_cap;
    while (new_cap <= n)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)7;

    /* initialize cache on first use */
    if (cache == NULL) {
        ensure_cache_cap(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache_used = 2;
    }

    /* compute up to n if needed */
    if (n >= cache_used) {
        ensure_cache_cap(n);
        for (idx_t i = cache_used; i <= n; ++i) {
            cache[i] = (seq_t)3 * cache[i - 1] + (seq_t)8 * cache[i - 2];
        }
        cache_used = n + 1;
    }

    return cache[n];
}
