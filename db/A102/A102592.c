/*
 * A102592  a(n) = Sum_{k=0..n} binomial(2n+1, 2k)*5^(n-k).
 * https://oeis.org/A102592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 12*a(n-1) - 16*a(n-2), a(0) = 1, a(1) = 8
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   resize_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/resize_array.h"

seq_t a(idx_t n) {
    /* static cache of computed terms */
    static seq_t *cache       = NULL;
    static idx_t cache_size   = 0;

    /* Ensure base values are cached */
    if (cache_size == 0) {
        /* allocate space for indices 0 and 1 */
        cache = resize_array(cache, cache_size, 2);
        if (!cache) return (seq_t)0;   /* allocation failure: return 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
        cache_size = 2;
        if (n < 2) return cache[n];
    }

    /* Expand cache up to index n */
    if (cache_size <= n) {
        /* Grow array to accommodate n+1 elements */
        cache = resize_array(cache, cache_size, n + 1);
        if (!cache) return (seq_t)0;   /* allocation failure */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t val = 12 * cache[i - 1] - 16 * cache[i - 2];
            cache[i] = val;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
