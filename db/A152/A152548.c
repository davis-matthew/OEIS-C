/*
 * A152548  Sum of squared terms in rows of triangle A152547: a(n) =
 *          Sum_{k=0..C(n,[n/2])-1} A152547(n,k)^2.
 * https://oeis.org/A152548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   (n+1)*a(n+1) = 4*a(n) + 4*n*a(n-1) a(0) = 1, a(1) = 4
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 24, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* ensure cache has enough space */
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* allocation failed; caller must handle */
        cache = new_cache;
        /* initialize new positions if extending beyond current size */
        for (idx_t i = cache_size; i < new_size; ++i) {
            cache[i] = (seq_t)0; /* will be overwritten below */
        }
        cache_size = new_size;
    }

    /* base cases */
    if (cache[0] == 0) cache[0] = (seq_t)1; /* a(0) = 1 */
    if (cache_size >= 2 && cache[1] == 0) cache[1] = (seq_t)4; /* a(1) = 4 */

    /* compute values up to n */
    for (idx_t i = (idx_t)2; i <= n; ++i) {
        if (cache[i] != 0) continue; /* already computed */
        seq_t term1 = (seq_t)4 * cache[i - 1];
        seq_t term2 = (seq_t)4 * (seq_t)(i - 1) * cache[i - 2];
        seq_t numerator = term1 + term2;
        cache[i] = numerator / (seq_t)i;
    }

    /* handle n == 1 specially if cache[1] not set yet */
    if (n == 1 && cache[1] == 0) {
        cache[1] = (seq_t)4;
    }

    return cache[n];
}
