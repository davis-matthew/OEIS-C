/*
 * A159537  Numerator of Hermite(n, 9/17).
 * https://oeis.org/A159537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 18*a(n-1) - 578*(n-1)*a(n-2), a(0)=1, a(1)=18
 * Range:     i8 n <= 1, i16 n <= 4, i32 n <= 6, i64 n <= 11, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cache_capacity(idx_t n) {
    size_t needed = (size_t)(n + 1);
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();        /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* seq_t a(idx_t n) - numerator of Hermite(n, 9/17) */
seq_t a(idx_t n) {
    if (n < cache_len) return cache[n];
    ensure_cache_capacity(n);
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[i] = 1;
        } else if (i == 1) {
            cache[i] = 18;
        } else {
            seq_t term1 = (seq_t)18 * cache[i - 1];
            seq_t term2 = (seq_t)578 * (seq_t)(i - 1) * cache[i - 2];
            cache[i] = term1 - term2;
        }
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
