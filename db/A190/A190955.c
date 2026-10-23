/*
 * A190955  a(n) = 10*a(n-1) + 5*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A190955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) + 5*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/ensure_capacity.h"

static seq_t *cache   = NULL;  /* cached values of the sequence */
static size_t cache_size = 0;  /* number of values currently cached */
static size_t cache_cap  = 0;  /* allocated capacity of the cache */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 0; we need indices 0 … n */
    const size_t needed = (size_t)n + 1;

    /* Grow the cache if necessary */
    if (needed > cache_cap)
        ensure_capacity(needed, &cache_cap, (void **)&cache, sizeof(seq_t));

    /* Initialise base cases if the cache is empty */
    if (cache_size == 0) {
        cache[0] = (seq_t)0;
        cache_size = 1;
    }

    /* If we need a(1) and it hasn't been computed yet, compute it */
    if (cache_size == 1 && needed > 1) {
        cache[1] = (seq_t)1;
        cache_size = 2;
    }

    /* Compute any new terms up to index n using the recurrence */
    if (cache_size < needed) {
        const seq_t TEN  = (seq_t)10;
        const seq_t FIVE = (seq_t)5;
        for (size_t i = cache_size; i < needed; ++i) {
            cache[i] = TEN * cache[i-1] + FIVE * cache[i-2];
        }
        cache_size = needed;
    }

    return cache[(size_t)n];
}
