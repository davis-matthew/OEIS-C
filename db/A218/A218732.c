/*
 * A218732  a(n) = (29^n - 1)/28.
 * https://oeis.org/A218732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (29^n - 1)/28
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t max_computed = 0;

    /* Ensure the array can hold index n */
    if ((size_t)n >= cache_size) {
        size_t new_size = cache_size ? cache_size : 2;
        while (new_size <= (size_t)n) new_size <<= 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache_size = new_size;
    }

    /* Initialize base values a(0)=0, a(1)=1 */
    if (max_computed < 1) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        max_computed = 1;
    }

    if (n <= max_computed) return cache[(size_t)n];

    for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)(30 * cache[i - 1] - 29 * cache[i - 2]);
    }
    max_computed = n;
    return cache[(size_t)n];
}
