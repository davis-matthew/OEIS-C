/*
 * A017828  Expansion of 1/(1 - x^4 - x^5 - x^6).
 * https://oeis.org/A017828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4) + a(n-5) + a(n-6); a(0) = 1, a(1) = 0, a(2) = 0,
 *            a(3) = 0, a(4) = 1, a(5) = 1, a(6) = 1
 * Range:     u8 n <= 32, u16 n <= 56, u32 n <= 106, u64 n <= 205,
 *            u128 n <= 300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* cached values */
    static idx_t cache_size = 0;  /* number of elements in cache */

    /* If already computed, return immediately */
    if (n < cache_size)
        return cache[n];

    /* Extend the cache to size n+1 */
    idx_t new_size = n + 1;
    size_t alloc_bytes = (size_t)new_size * sizeof(seq_t);
    seq_t *tmp = (seq_t *)realloc(cache, alloc_bytes);
    if (!tmp)                /* allocation failure – fall back to 0 */
        return (seq_t)0;
    cache = tmp;

    /* Compute all new entries in ascending order */
    for (idx_t i = cache_size; i < new_size; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i < 4) {
            cache[i] = (seq_t)0;
        } else if (i <= 6) {
            cache[i] = (seq_t)1;
        } else {
            cache[i] = cache[i-4] + cache[i-5] + cache[i-6];
        }
    }

    cache_size = new_size;
    return cache[n];
}
