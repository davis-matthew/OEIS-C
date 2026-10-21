/*
 * A081039  4th binomial transform of (1,3,0,0,0,0,0,.....).
 * https://oeis.org/A081039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n−1) − 16*a(n−2), a(0) = 1, a(1) = 7. (This is the
 *            4‑th binomial transform of (1,3,0,0,0,…)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Ensure cache has at least two elements for the base cases */
    if (cache_len < 2) {
        size_t new_len = 2;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* allocation failure – unlikely */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache_len = new_len;
    }

    /* Return immediately if the requested index is already cached */
    if ((size_t)n < cache_len) {
        return cache[(size_t)n];
    }

    /* Expand cache to accommodate n */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (tmp == NULL) return (seq_t)0;  /* allocation failure – unlikely */
    cache = tmp;
    /* Compute terms iteratively up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* a(i) = 8*a(i-1) - 16*a(i-2) */
        seq_t val = cache[i - 1];
        val *= (seq_t)8;
        seq_t sub = cache[i - 2];
        sub *= (seq_t)16;
        val -= sub;
        cache[i] = val;
    }
    cache_len = new_len;
    return cache[(size_t)n];
}
