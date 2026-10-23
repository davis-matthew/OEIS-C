/*
 * A026324  Expansion of 1/((1-2*x)*(1-6*x)*(1-7*x)*(1-8*x)).
 * https://oeis.org/A026324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 23*a(n-1) - 188*a(n-2) + 628*a(n-3) - 672*a(n-4),
   with a(0) = 1, a(1) = 23, a(2) = 341, a(3) = 4147. */
#include <stddef.h>
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static void init_cache(void) {
    cache_len = 4;
    cache = seq_realloc(NULL, 0, (size_t)cache_len);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)23;
    cache[2] = (seq_t)341;
    cache[3] = (seq_t)4147;
}

seq_t a(idx_t n) {
    if (cache == NULL) init_cache();
    if (n < cache_len) return cache[n];

    size_t new_len = (size_t)(n + 1);
    cache = seq_realloc(cache, cache_len, new_len);
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t v1 = cache[i - 1];
        seq_t v2 = cache[i - 2];
        seq_t v3 = cache[i - 3];
        seq_t v4 = cache[i - 4];
        cache[i] = 23 * v1 - 188 * v2 + 628 * v3 - 672 * v4;
    }
    cache_len = new_len;
    return cache[(size_t)n];
}
