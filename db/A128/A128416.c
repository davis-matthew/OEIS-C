/*
 * A128416  Expansion of (1-4x^2)/(1+4x+3x^2).
 * https://oeis.org/A128416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -4*a(n-1)-3*a(n-2) (for n>2), a(0)=1, a(1)=-4, a(2)=9
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 19, i64 n <= 39, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed terms */
static idx_t cache_len = 0;          /* number of terms stored (valid indices 0..cache_len-1) */
static size_t cache_cap = 0;         /* allocated capacity */

/* Ensure the cache can hold at least (n+1) elements, grow if needed */
static void ensure_cache(idx_t n)
{
    if (cache_len > n) return;

    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
    if (cache_len == 0) {            /* initialize the first three terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-4;
        cache[2] = (seq_t)9;
        cache_len = 3;
    }
}

/* Computes a(n) for any idx_t n >= 0 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-4;
    if (n == 2) return (seq_t)9;

    ensure_cache(n);

    if ((idx_t)cache_len <= n) {
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t v = -(cache[i-1] * (seq_t)4) - (cache[i-2] * (seq_t)3);
            cache[i] = v;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
