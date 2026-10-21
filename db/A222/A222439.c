/*
 * A222439  Number of n X 3 0..3 arrays with entries increasing mod 4 by 0, 1
 *          or 2 rightwards and downwards, starting with upper left zero.
 * https://oeis.org/A222439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=9, a(2)=147. a(n)=18*a(n-1)-27*a(n-2) for n≥3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (n == 0) return (seq_t)0;        /* undefined, return 0 */

    /* initialise cache with first two terms if not already done */
    if (cache_len < 2) {
        cache_len = 2;
        cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        cache[0] = (seq_t)9;
        cache[1] = (seq_t)147;
    }

    size_t target = (size_t)n;          /* n ≥ 1, fits in size_t for all expected inputs */

    /* extend cache up to the required term */
    if (target > cache_len) {
        seq_t *new_cache = (seq_t *)realloc(cache, target * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* allocation failed */
        cache = new_cache;
        for (size_t i = cache_len; i < target; ++i) {
            cache[i] = (seq_t)18 * cache[i-1] - (seq_t)27 * cache[i-2];
        }
        cache_len = target;
    }

    return cache[n-1];   /* cache is 0‑based: a(1) -> cache[0] */
}
