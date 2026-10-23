/*
 * A144033  a(n) = (a(n-1)+2)^2 - n^3 with a(0) = 0.
 * https://oeis.org/A144033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1)+2)^2 - n^3 a(0) = 0
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoization */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_size = 0;

static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap;
        while ((idx_t)new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return a(n) for any n >= 0, in any order of calls */
seq_t a(idx_t n)
{
    if (n >= cache_size) {
        ensure_cache(n);
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 0;
            } else {
                seq_t prev = cache[i - 1];
                seq_t tmp = prev + (seq_t)2;
                seq_t square = tmp * tmp;
                seq_t idx = (seq_t)i;
                seq_t cube = idx * idx * idx;
                cache[i] = square - cube;
            }
        }
        cache_size = n + 1;
    }
    return cache[n];
}
