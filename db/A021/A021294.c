/*
 * A021294  Expansion of 1/((1-x)(1-2x)(1-9x)(1-11x)).
 * https://oeis.org/A021294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 167 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A021294: a(n) = 23*a(n-1) -161*a(n-2) +337*a(n-3) -198*a(n-4)
   base values: a(0)=1, a(1)=23, a(2)=368, a(3)=5098 */

#include <stddef.h>     /* size_t */
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

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache   = NULL;
    static size_t cap      = 0;
    static idx_t  last = (idx_t)-1;   /* last computed index */

    /* initialise cache with first four terms on the first call */
    if (last == (idx_t)-1) {
        /* ensure capacity for indices 0..3 */
        ensure_cache(3, &cache, &cap);   /* may allocate more than needed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)23;
        cache[2] = (seq_t)368;
        cache[3] = (seq_t)5098;
        last = 3;
    }

    /* already known */
    if (n <= last)
        return cache[n];

    /* extend cache to include index n */
    ensure_cache(n, &cache, &cap);

    /* compute new terms using the 4th‑order linear recurrence */
    for (idx_t k = last + 1; k <= n; ++k) {
        seq_t term = 23 * cache[k - 1] + 337 * cache[k - 3]
                   - (161 * cache[k - 2] + 198 * cache[k - 4]);
        cache[k] = term;
    }
    last = n;
    return cache[n];
}
