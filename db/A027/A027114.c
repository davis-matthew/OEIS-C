/*
 * A027114  a(n) = A027113(n, n+2).
 * https://oeis.org/A027114
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n‑1) – a(n‑4) (n≥4) with a(0) = –2, a(1) = 0, a(2) =
 *            3, a(3) = 9. This recurrence generates OEIS A027114; we cache
 *            computed values so that non‑increasing calls are handled
 *            efficiently
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* dynamic cache of sequence values */
static idx_t cache_size = 0;     /* length of allocated cache   */

/* Extend the cache so that it contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        idx_t size = (n > 3) ? (n + 1) : 4;               /* at least 4 slots */
        cache = (seq_t *)malloc(size * sizeof(seq_t));
        cache_size = size;
        if (!cache) return;                              /* allocation failed */

        /* initialise the first four terms (a0..a3) */
        cache[0] = (seq_t)-2;   /* two's‑complement representation of –2 */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)9;
        for (idx_t i = 4; i < cache_size; ++i) {
            cache[i] = cache[i - 1] + cache[i - 1] - cache[i - 4];
        }
    } else if (n >= cache_size) {
        /* grow the cache to hold indices up to n */
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return;          /* allocation failed, keep old cache */
        cache = tmp;
        /* compute new entries using the recurrence */
        for (idx_t i = cache_size; i < new_size; ++i) {
            cache[i] = cache[i - 1] + cache[i - 1] - cache[i - 4];
        }
        cache_size = new_size;
    }
}

seq_t a(idx_t n)
{
    /* we only need to support n ≥ 2, but cache handles 0..n anyway */
    ensure_cache(n);
    return cache[n];
}
