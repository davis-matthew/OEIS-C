/*
 * A117727  Partial sums of A051109.
 * https://oeis.org/A117727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 10*a(n-3) – 10*a(n-4) Initial values: a(0)=1,
 *            a(1)=3, a(2)=8, a(3)=18
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 56, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (!cache) {
        // Ensure at least 4 entries for the base cases
        cache_size = (n + 1 > 4 ? n + 1 : 4);
        cache = (seq_t *)malloc((size_t)cache_size * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)18;

        for (idx_t i = 4; i < cache_size; ++i) {
            cache[i] = cache[i-1] + (seq_t)10 * cache[i-3] - (seq_t)10 * cache[i-4];
        }
        return cache[n];
    }

    if (n >= cache_size) {
        idx_t new_size = n + 1; /* at least up to n */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed */
        cache = tmp;

        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = cache[i-1] + (seq_t)10 * cache[i-3] - (seq_t)10 * cache[i-4];
        }
        cache_size = new_size;
    }

    return cache[n];
}
