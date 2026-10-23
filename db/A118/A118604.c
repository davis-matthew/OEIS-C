/*
 * A118604  a(n) = a(n-1) mod 13 + a(n-2) mod 8, with a(0) = 0 and a(1) = 1.
 * https://oeis.org/A118604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = a(n-1) mod 13 + a(n-2) mod 8, a(0) = 0, a(1) = 1 */
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void init_cache(void) {
    if (cache) return;
    cache_cap = 2;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) abort();          /* impossible for small size */
    cache[0] = (seq_t)0;
    cache[1] = (seq_t)1;
    cache_len = 2;
}

static void ensure_cache_size(size_t needed) {
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap;
    if (new_cap == 0) new_cap = 2;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    init_cache();

    size_t idx = (size_t)n;
    ensure_cache_size(idx + 1);

    if (idx >= cache_len) {
        for (size_t i = cache_len; i <= idx; ++i) {
            cache[i] = (cache[i-1] % (seq_t)13) + (cache[i-2] % (seq_t)8);
        }
        cache_len = idx + 1;
    }
    return cache[idx];
}
