/*
 * A116415  a(n) = 5*a(n-1) - 3*a(n-2), with a(0) = 1 and a(1) = 5.
 * https://oeis.org/A116415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) - 3*a(n-2); a(0)=1, a(1)=5
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Cache to allow arbitrary call order and reuse of previous results. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* First call: allocate and initialise the first two terms. */
    if (cache == NULL) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – unlikely */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
    }

    size_t idx = (size_t)n;

    /* Return a cached value if already known. */
    if (idx < cache_len) {
        return cache[idx];
    }

    /* Extend the cache to accommodate the new index. */
    size_t old_len = cache_len;
    size_t needed = idx + 1;
    if (needed > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure – unlikely */
        cache = tmp;
        cache_len = needed;
    }

    /* Compute the missing terms using the recurrence. */
    for (size_t i = old_len; i <= idx; ++i) {
        seq_t val = ((seq_t)5 * cache[i - 1]) - ((seq_t)3 * cache[i - 2]);
        cache[i] = val;
    }

    return cache[idx];
}
