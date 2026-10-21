/*
 * A052129  a(0) = 1; thereafter a(n) = n*a(n-1)^2.
 * https://oeis.org/A052129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(0) = 1; thereafter a(n) = n*a(n-1)^2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static size_t cache_cap = 0;

static void grow_cache(idx_t new_len)
{
    if (new_len <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < (size_t)new_len) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        if (cache_len == 0) {
            grow_cache(1);
            cache[0] = (seq_t)1;
            cache_len = 1;
        }
        return cache[0];
    }

    if (n >= cache_len) {
        grow_cache(n + 1);
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t sq = prev * prev;
            cache[i] = (seq_t)i * sq;
        }
        cache_len = n + 1;
    }
    return cache[n];
}
