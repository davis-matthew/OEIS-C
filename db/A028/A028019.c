/*
 * A028019  Expansion of 1/((1-2x)(1-8x)(1-10x)(1-12x)).
 * https://oeis.org/A028019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*12^(n+3)-15*10^(n+3)+10*8^(n+3)-2^(n+3))/480
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"


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
