/*
 * A138749  a(n) = 2*a(n-1) - 5*a(n-2), with a(1) = -1, a(2) = -7.
 * https://oeis.org/A138749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) - 5*a(n-2); a(1) = -1, a(2) = -7, a(3) = -9
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 26, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

static void init_cache(void) {
    if (cache != NULL) return;
    cache = (seq_t*)malloc(sizeof(seq_t) * 4); /* indices 1..3 used */
    if (!cache) return; /* memory error – return 0 (undefined for build) */
    cache[1] = (seq_t)-1;
    cache[2] = (seq_t)-7;
    cache[3] = (seq_t)-9;
    cache_size = 3;
}

seq_t a(idx_t n) {
    init_cache();
    if (n <= 3) return cache[n];
    if (n > cache_size) {
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!tmp) return (seq_t)0; /* memory error */
        cache = tmp;
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            cache[i] = 2 * cache[i - 1] - 5 * cache[i - 2];
        }
        cache_size = n;
    }
    return cache[n];
}
