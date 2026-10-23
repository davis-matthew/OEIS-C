/*
 * A133667  a(n) = a(n-1) - 25*a(n-2), a(0)=1, a(1)=5.
 * https://oeis.org/A133667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) - 25*a(n-2), a(0) = 1, a(1) = 5
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 13, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* static cache for memoization */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_len = 0;

/* ensure the cache can hold values up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n < cache_len) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!tmp) abort();
    if (cache) {
        memcpy(tmp, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* public function */
seq_t a(idx_t n)
{
    ensure_cache(n);

    if (n >= cache_len) {
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
            cache_len = 1;
        }
        if (cache_len == 1 && n >= 1) {
            cache[1] = (seq_t)5;
            cache_len = 2;
        }
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = cache[i-1] - (seq_t)25 * cache[i-2];
        }
        cache_len = n + 1;
    }
    return cache[n];
}
