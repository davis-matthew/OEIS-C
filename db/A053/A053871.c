/*
 * A053871  a(n) = 2*(n-1)*(a(n-1) + a(n-2)), starting a(0) = 1; a(1) = 0.
 * https://oeis.org/A053871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 2*(n-1)*(a(n-1) + a(n-2)), a(0) = 1, a(1) = 0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoised cache of already–computed values. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements currently stored
                                      (cache_len = max index + 1)    */
    static size_t cache_cap = 0;   /* allocated capacity of the cache     */

    /* Grow the cache until it can hold index `n`. */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        /* cast for C++ compatibility */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();          /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise base values if the cache is currently empty. */
    if (cache_len <= 1) {
        if (cache_len == 0) cache[0] = (seq_t)1;
        if (cache_len <= 1) cache[1] = (seq_t)0;
        cache_len = 2;
    }

    /* Compute subsequent values up to `n`. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t sum   = cache[i-1] + cache[i-2];
        seq_t factor = ((seq_t)(i-1)) * (seq_t)2;
        cache[i] = sum * factor;
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
