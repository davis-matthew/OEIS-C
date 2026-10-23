/*
 * A015598  a(n) = 11*a(n-1) + 6*a(n-2).
 * https://oeis.org/A015598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) + 6*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_cap = 0;

static void ensure_cache(idx_t n)
{
    if (cache_size > n)
        return;

    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) abort();
    }

    while (cache_cap <= n) {
        cache_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
        if (!tmp) abort();
        cache = tmp;
    }

    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[i] = 0;
        } else if (i == 1) {
            cache[i] = 1;
        } else {
            cache[i] = cache[i - 1] * 11 + cache[i - 2] * 6;
        }
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
