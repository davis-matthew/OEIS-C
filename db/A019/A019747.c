/*
 * A019747  Expansion of 1/((1-4x)(1-10x)(1-12x)).
 * https://oeis.org/A019747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 26*a(n-1) - 208*a(n-2) + 480*a(n-3) a(0) = 1, a(1) = 26,
 *            a(2) = 468 (The recurrence holds for all n ≥ 3)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    size_t idx = (size_t)n;

    /* initialise cache with the first three terms */
    if (cache_len == 0) {
        cache_cap = 4;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;           /* allocation failed */
        cache[0] = 1;
        cache[1] = 26;
        cache[2] = 468;
        cache_len = 3;
    }

    /* if requested index already computed, return it */
    if (idx < cache_len)
        return cache[idx];

    /* grow cache if necessary */
    if (idx >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= idx) new_cap <<= 1;   /* double until big enough */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                     /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (size_t i = cache_len; i <= idx; ++i) {
        seq_t val = cache[i-1] * ((seq_t)26)
                  - cache[i-2] * ((seq_t)208)
                  + cache[i-3] * ((seq_t)480);
        cache[i] = val;
    }
    cache_len = idx + 1;

    return cache[idx];
}
