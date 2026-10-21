/*
 * A099134  Expansion of x/(1-2*x-19*x^2).
 * https://oeis.org/A099134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 19*a(n-2); a(0)=0, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache      = NULL; /* stores computed terms */
static idx_t  cache_len  = 0;     /* allocated size */
static idx_t  terms_done = 0;     /* number of terms computed (>=0) */

/* Ensure that the cache can hold at least 'size' elements */
static void ensure_cache_size(idx_t size) {
    if (cache_len >= size) return;
    idx_t new_len = (cache_len == 0) ? 16 : cache_len * 2;
    while (new_len < size) new_len <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) { /* Allocation failed – abort program (no error handling needed) */
        abort();
    }
    cache = tmp;
    cache_len = new_len;
}

seq_t a(idx_t n) {
    /* Initial terms */
    if (terms_done == 0) {
        ensure_cache_size(2);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        terms_done = 2;
    }
    if (n < terms_done) {
        return cache[n];
    }
    /* Compute missing terms up to n */
    ensure_cache_size((size_t)(n + 1));
    for (idx_t i = terms_done; i <= n; ++i) {
        cache[i] = (seq_t)2 * cache[i - 1] + (seq_t)19 * cache[i - 2];
    }
    terms_done = n + 1;
    return cache[n];
}
