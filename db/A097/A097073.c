/*
 * A097073  Expansion of (1-x+2*x^2)/((1+x)*(1-2*x)).
 * https://oeis.org/A097073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*2^n + 4*(-1)^n)/3 - 0^n Recurrence for n ≥ 2: a(n) =
 *            2*a(n-1) + 4*(-1)^n Initial values: a(0) = 1, a(1) = 0
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External linkage */
seq_t a(idx_t n)
{
    /* Static cache for previously computed values */
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;

    /* Bootstrap cache with a(0) and a(1) */
    if (cache == NULL) {
        cached_len = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cached_len);
        if (!cache) { /* allocation failed – return 0 as a safe fallback */
            return (seq_t)0;
        }
        cache[0] = (seq_t)1;      /* a(0) */
        cache[1] = (seq_t)0;      /* a(1) */
    }

    /* If the requested index is already cached, return it directly */
    if (n < cached_len) {
        return cache[n];
    }

    /* Expand the cache up to n */
    idx_t new_len = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
    if (!new_cache) {
        /* realloc failed – keep old cache */
        return (seq_t)0;
    }
    cache = new_cache;

    /* Compute values from the current cached length up to n */
    for (idx_t i = cached_len; i < new_len; ++i) {
        seq_t prev = cache[i - 1];
        seq_t next;
        if ((i & (idx_t)1) == 0) {        /* i even: (+4) */
            next = prev * (seq_t)2 + (seq_t)4;
        } else {                          /* i odd:  (‑4) */
            next = prev * (seq_t)2 - (seq_t)4;
        }
        cache[i] = next;
    }
    cached_len = new_len;

    return cache[n];
}
