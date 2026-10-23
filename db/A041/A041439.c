/*
 * A041439  Denominators of continued fraction convergents to sqrt(235).
 * https://oeis.org/A041439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 92*a(n-2) - a(n-4) Initial values: a(0)=1, a(1)=3,
 *            a(2)=91, a(3)=276
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;

seq_t a(idx_t n) {
    /* Ensure cache is initialized to cover at least indices 0-3 */
    if (cache_size == 0) {
        cache = (seq_t *)realloc(NULL, 4 * sizeof(seq_t));
        if (!cache) return 0;          /* out of memory: return 0 as a fallback */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)91;
        cache[3] = (seq_t)276;
        cache_size = 4;
    }

    /* If requested index is already calculated, just return it. */
    if ((size_t)n < cache_size) {
        return cache[n];
    }

    /* Extend cache to cover the requested index. */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return 0;          /* out of memory: return 0 as a fallback */
    cache = tmp;

    /* Compute missing entries up to n. */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        /* Since a(0) .. a(3) are already set, compute from i=4 onwards. */
        cache[i] = 92 * cache[i - 2] - cache[i - 4];
    }
    cache_size = new_size;

    return cache[n];
}
