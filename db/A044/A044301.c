/*
 * A044301  Numbers n such that string 5,5 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains55
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n-th positive integer whose base‑9 representation
 * contains the consecutive digits 5,5 but the previous integer does not.
 * The function generates terms on demand and caches them for future calls. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/contains55.h"

/* Cached results. */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_len = 0;
static idx_t last_checked = 0;  /* last integer examined */

/* Grow the cache to accommodate at least 'needed' elements. */
static void ensure_capacity(idx_t needed)
{
    if (needed <= cache_cap) return;

    idx_t new_cap = cache_cap < 128 ? 128 : cache_cap * 2;
    if (new_cap < needed) new_cap = needed;

    if (cache == NULL) {
        cache = (seq_t *)malloc((size_t)new_cap * sizeof(seq_t));
    } else {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (tmp != NULL) cache = tmp;
    }
    cache_cap = new_cap;
}

/* Public API: compute the nth term of the sequence. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;  /* undefined, return 0 */

    /* If the requested term is already cached, return it directly. */
    if (n <= cache_len) return cache[n - 1];

    /* Otherwise generate further terms until we reach the requested index. */
    while (cache_len < n) {
        last_checked++;
        seq_t val = (seq_t)last_checked;
        if (!contains55(val)) continue;

        seq_t prev_val = val - (seq_t)1;
        if (contains55(prev_val)) continue;

        ensure_capacity(cache_len + 1);
        cache[cache_len++] = val;
    }

    return cache[n - 1];
}
