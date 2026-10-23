/*
 * A189706  Fixed point of the morphism 0->011, 1->001.
 * https://oeis.org/A189706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(3k-2)=0, a(3k-1)=1-a(k), a(3k)=1 (k≥1), a(0)=0
 * Range:     u8 n <= 122, u16 n <= 122, u32 n <= 122, u64 n <= 122,
 *            u128 n <= 122, big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (The recurrence gives the fixed‑point sequence of the morphism
 *   0→011, 1→001.) */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/alloc_seq.h"   /* provides seq_t *alloc_seq(size_t) */

static seq_t  *cache   = NULL;   /* computed values (len >= cache_cap) */
static uint8_t *computed = NULL; /* flag array: 1 = value present */
static size_t   cache_cap = 0;   /* allocated capacity */

/* Grow the cache to hold at least index n (0‑based). */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    seq_t  *new_cache = alloc_seq(new_cap);
    uint8_t *new_comp = (uint8_t*)calloc(new_cap, sizeof(uint8_t));

    if (cache)
    {
        memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
        memcpy(new_comp, computed, cache_cap * sizeof(uint8_t));
        free(cache);
        free(computed);
    }
    cache    = new_cache;
    computed = new_comp;
    cache_cap = new_cap;
}

/* Recursive computation with memoisation. */
static seq_t seq_compute(idx_t n)
{
    if (n == 0) return 0;           /* base value, not used for offset ≥1 */
    ensure_cache(n);
    if (computed[n]) return cache[n];

    seq_t result;
    switch (n % 3)
    {
        case 1: /* a(3k-2) = 0 */
            result = 0;
            break;
        case 0: /* a(3k)   = 1 */
            result = 1;
            break;
        case 2: /* a(3k-1) = 1 - a(k) */
        {
            idx_t k = (n + 1) / 3;
            result = 1 - seq_compute(k);
            break;
        }
    }
    cache[n] = result;
    computed[n] = 1;
    return result;
}

/* Public interface required by the platform. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence is defined for n ≥ 1 */
    return seq_compute(n);
}
