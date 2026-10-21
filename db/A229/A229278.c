/*
 * A229278  Number of ascending runs in {1,...,4}^n.
 * https://oeis.org/A229278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 16*a(n-2) for n > 2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache of previously computed values */
static seq_t *cache = NULL;   /* array of a(0) .. a(cache_len-1)   */
static idx_t  cache_len = 0;  /* number of valid elements           */
static idx_t  cache_cap = 0;  /* allocated capacity (>= cache_len) */

/* Ensure that the cache has at least entries 0..n, computing new ones if needed. */
static void ensure_cache(idx_t n)
{
    /* First allocation and initialization */
    if (cache == NULL) {
        /* Minimal capacity to hold the initial terms 0,1,2 */
        cache_cap = (n < 2 ? 3 : n + 1);
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL) exit(1);          /* allocation failure */

        /* Known base values */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)26;
        cache_len = (idx_t)3;                /* we have a(0),a(1),a(2) */
    }

    /* Grow the cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;   /* exponential growth */

        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) exit(1);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence; starts from cache_len */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* Recurrence is valid for i >= 3; for i < 3 values were preset */
        if (i >= 3) {
            cache[i] = (seq_t)(8 * cache[i - 1] - 16 * cache[i - 2]);
        }
    }
    cache_len = n + 1;        /* update the known length */
}

/* External function requested by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* early out for the offset 0 */
    ensure_cache(n);
    return cache[n];
}
