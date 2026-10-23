/*
 * A171842  Binomial transform of 1,0,1,0,2,0,4,0,8,0,16,...
 * https://oeis.org/A171842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1)-a(n-2)-a(n-3) Initial values: a(0)=1, a(1)=1,
 *            a(2)=2
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 26, u64 n <= 51, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;

    /* Expand cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
        while ((size_t)n + 1 > new_cap) new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failed, return 0 as error */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)1;
        } else if (i == 2) {
            cache[i] = (seq_t)2;
        } else {
            seq_t t = cache[i-1];
            t *= (seq_t)3;
            t -= cache[i-2];
            t -= cache[i-3];
            cache[i] = t;
        }
    }
    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
