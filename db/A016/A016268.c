/*
 * A016268  Expansion of 1/((1-x)(1-11x)(1-12x)).
 * https://oeis.org/A016268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*a(n-1) - 155*a(n-2) + 132*a(n-3) Initial values: a(0)
 *            = 1, a(1) = 24, a(2) = 421
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t computed_up_to = 2; /* indices 0..2 are known */

/* Return a(n) for OEIS A016268 */
seq_t a(idx_t n)
{
    /* Direct return for initial indices */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)24;
    if (n == 2) return (seq_t)421;

    /* First allocation: store the known initial values */
    if (cache == NULL) {
        cache_cap = 3;
        cache = seq_alloc(cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)24;
        cache[2] = (seq_t)421;
    }

    /* Grow the cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until large enough */
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    if ((size_t)n > computed_up_to) {
        for (size_t i = computed_up_to + 1; i <= (size_t)n; ++i) {
            cache[i] = (24 * cache[i-1]) - (155 * cache[i-2]) + (132 * cache[i-3]);
        }
        computed_up_to = (size_t)n;
    }

    return cache[n];
}
