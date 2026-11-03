/*
 * A028019  Expansion of 1/((1-2x)(1-8x)(1-10x)(1-12x)).
 * https://oeis.org/A028019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*12^(n+3)-15*10^(n+3)+10*8^(n+3)-2^(n+3))/480
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-03)
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

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_size = 0;

static const seq_t C1 = (seq_t)32;
static const seq_t C2 = (seq_t)356;
static const seq_t C3 = (seq_t)1552;
static const seq_t C4 = (seq_t)1920;

static void init_cache(void) {
    /* Ensure space for indices 0..3 (four terms) */
    ensure_cache(3, &cache, &cache_cap);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)32;
    cache[2] = (seq_t)668;
    cache[3] = (seq_t)11536;
    cache_size = 3;
}

seq_t a(idx_t n) {
    if (!cache) init_cache();

    if (n <= cache_size) return cache[n];

    /* Grow cache if needed */
    if (n > cache_cap - 1) {
        ensure_cache(n, &cache, &cache_cap);
    }

    for (idx_t i = cache_size + 1; i <= n; ++i) {
        /* Compute terms using the recurrence:
           a(i) = 32*a(i-1) - 356*a(i-2) + 1552*a(i-3) - 1920*a(i-4)  */
        seq_t up   = cache[i-1] * C1 + cache[i-3] * C3;  /* positive part */
        seq_t down = cache[i-2] * C2 + cache[i-4] * C4;  /* positive part */
        cache[i] = up - down;                            /* guaranteed non‑negative */
    }
    cache_size = n;
    return cache[n];
}
