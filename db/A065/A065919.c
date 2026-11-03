/*
 * A065919  Bessel polynomial y_n(4).
 * https://oeis.org/A065919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(2*n-1)*a(n-1) + a(n-2), with a(0)=1, a(1)=5
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 100 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Alias for arbitrary‑precision type in the build */
#include <stdlib.h>
#include "../utils/mem.h"

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

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t  cache_last = (idx_t)-1;   /* index of last stored term */

/* Compute the n‑th term of A065919 */
seq_t a(idx_t n)
{
    /* Initialisation: ensure base terms are present */
    if (cache == NULL) {
        /* guarantee capacity for indices 0 and 1 */
        ensure_cache(1, &cache, &cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache_last = 1;
    }

    /* If the cache already contains the desired term, return it */
    if (n <= cache_last)
        return cache[n];

    /* Expand the cache if necessary */
    ensure_cache(n, &cache, &cache_cap);

    /* Compute new terms up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t term = 4 * ((seq_t)(2 * i - 1)) * cache[i - 1] + cache[i - 2];
        cache[i] = term;
    }
    cache_last = n;
    return cache[n];
}
