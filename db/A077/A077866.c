/*
 * A077866  Expansion of (1-x)^(-1)/(1 - x - 2*x^2 + 2*x^3).
 * https://oeis.org/A077866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n‑1)·2 + a(n‑2) – a(n‑3)·4 + a(n‑4)·2, with a(0) = 1,
 *            a(1) = 2, a(2) = 5, a(3) = 8. This recurrence generates the OEIS
 *            sequence A077866. The function uses a static cache so that calls
 *            with arbitrary indices run in O(1) amortised time
 * Range:     u8 n <= 11, u16 n <= 27, u32 n <= 59, u64 n <= 123,
 *            u128 n <= 251, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;   /* pointer to allocated array      */
    static idx_t cache_len   = 0;      /* number of cached elements       */
    static idx_t cache_cap   = 0;      /* allocated capacity               */

    /* grow the cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = (cache_cap == 0) ? n + 1 : cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* out of memory – fallback          */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        switch (i) {
            case 0: val = 1; break;
            case 1: val = 2; break;
            case 2: val = 5; break;
            case 3: val = 8; break;
            default:
                val = 2*cache[i-1] + cache[i-2] - 4*cache[i-3] + 2*cache[i-4];
                break;
        }
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
