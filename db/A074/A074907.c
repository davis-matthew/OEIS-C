/*
 * A074907  Aliquot sequence starting at 570.
 * https://oeis.org/A074907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(0)=570, a(n+1)=sigma(a(n))-a(n)
 * Range:     u8 none, u16 n <= 16, u32 n <= 105, u64 n <= 173, u128 n <= 173,
 *            big any n
 * Validated: full, pass, 174 terms through n = 173 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Ensure we have space for up to n */
    if ((size_t)n >= cache_cap)
        ensure_cache(n, &cache, &cache_cap);

    /* First term */
    if (cache_len == 0)
    {
        cache[0] = 570;
        cache_len = 1;
    }

    /* Compute missing terms */
    size_t nidx = (size_t)n;
    for (size_t i = cache_len; i <= nidx; ++i)
    {
        seq_t prev = cache[i - 1];
        seq_t sigma = sigma_of((idx_t)prev);
        cache[i] = sigma - prev;
    }
    cache_len = nidx + 1;

    return cache[nidx];
}
