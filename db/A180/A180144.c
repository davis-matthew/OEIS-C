/*
 * A180144  Eight rooks and one berserker on a 3 X 3 chessboard. G.f.: (1 -
 *          2*x^2)/(1 - 4*x + x^2 + 2*x^3).
 * https://oeis.org/A180144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - 1*a(n-2) - 2*a(n-3); a(0)=1, a(1)=4, a(2)=13
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 23 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
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
    /* Static cache for memoization */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;         /* current capacity of cache */
    static size_t  max_cur = 2;        /* largest index already computed (a(2) is known) */
    static int     init_done = 0;      /* init flag */

    /* Initialisation of base values */
    if (!init_done) {
        ensure_cache(2, &cache, &cap);     /* allocate storage for indices 0..2 */
        cache[0] = 1;
        cache[1] = 4;
        cache[2] = 13;
        init_done = 1;
    }

    /* Grow cache if necessary */
    if ((idx_t)cap <= n) {
        ensure_cache(n, &cache, &cap);
    }

    /* Compute missing terms up to n */
    if ((idx_t)max_cur < n) {
        for (idx_t i = max_cur + 1; i <= n; ++i) {
            seq_t t1 = cache[i-1] * 4u;   /* 4*a(i-1)   */
            seq_t t2 = cache[i-3] * 2u;   /* 2*a(i-3)   */
            cache[i] = t1 - cache[i-2] - t2;
        }
        max_cur = n;
    }

    return cache[n];
}
