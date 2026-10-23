/*
 * A152103  a(n) = 2^n*Product_{k=1..floor((n-1)/2)} (1 + 2*cos(k*Pi/n)^2 +
 *          4*cos(k*Pi/n)^4).
 * https://oeis.org/A152103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A152103 (recurrence) */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

/* Ensure the cache can hold indices 0..need. */
static void ensure_capacity(idx_t need)
{
    if (need < cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= need) new_cap *= 2;
#ifdef __cplusplus
    seq_t *tmp = new seq_t[new_cap];
    for (idx_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];
    delete[] cache;
#else
    seq_t *tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!tmp) abort();
    if (cache) {
        memcpy(tmp, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
#endif
    cache = tmp;
    cache_cap = new_cap;
}

/* Returns a(n) for all n >= 0.  Uses memoisation. */
seq_t a(idx_t n)
{
    /* Initialise the first six terms on first use. */
    if (cache_len == 0) {
        ensure_capacity(5);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)14;
        cache[4] = (seq_t)48;
        cache[5] = (seq_t)158;
        cache_len = 6;
    }

    /* Extend cache up to the required index. */
    if (n >= cache_len) {
        ensure_capacity(n);
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val;
            if (i >= 6) {
                val = (cache[i-1] << 1) + (cache[i-2] << 2) + (cache[i-3] << 1) - cache[i-4];
            } else {
                val = cache[i];  /* Should never happen for i >= 6 */
            }
            cache[i] = val;
        }
        cache_len = n + 1;
    }
    return cache[n];
}
