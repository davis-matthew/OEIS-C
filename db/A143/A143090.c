/*
 * A143090  Aliquot sequence starting at 12.
 * https://oeis.org/A143090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=12, a(n+1)=sigma_of(a(n)) - a(n)
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   seq_alloc, seq_realloc, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/sigma_of.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static void ensure_cache(idx_t needed) {
    if (cache_len >= needed) return;
    if (cache == NULL) {
        cache = seq_alloc(needed);
    } else {
        cache = seq_realloc(cache, cache_len, needed);
    }
    cache_len = needed;
}

seq_t a(idx_t n) {
    if (cache_len == 0) {
        ensure_cache(1);
        cache[0] = (seq_t)12;
        cache_len = 1;
    }

    if (n < cache_len) return cache[n];

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t next;
        if ((idx_t)prev <= 1) {
            next = 0;
        } else {
            idx_t prev_small = (idx_t)prev;
            next = (seq_t)(sigma_of(prev_small) - prev_small);
        }
        ensure_cache(i + 1);
        cache[i] = next;
    }
    cache_len = n + 1;
    return cache[n];
}
