/*
 * A048876  a(n) = 4*a(n-1) + a(n-2); a(0)=1, a(1)=7.
 * https://oeis.org/A048876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) + a(n-2); a(0) = 1, a(1) = 7
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache of computed values */
static idx_t cache_size = 0;  /* current number of cached entries */

seq_t a(idx_t n) {
    /* handle the two base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;

    /* initialise cache if this is the first non‑base call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) abort();                /* out of memory */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache_size = 2;
    }

    /* return cached value if already computed */
    if (n < cache_size)
        return cache[n];

    /* extend cache up to index n */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();                      /* out of memory */
    cache = tmp;

    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = (seq_t)4 * cache[i-1] + cache[i-2];
    }
    cache_size = new_size;

    return cache[n];
}
