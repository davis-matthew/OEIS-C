/*
 * A016321  Expansion of 1/((1-2x)*(1-9x)*(1-10x)).
 * https://oeis.org/A016321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19*a(n-1) - 90*a(n-2) + 2^n (n >= 2), a(0) = 1, a(1) = 21
 *            (expansion of 1/((1-2x)*(1-9x)*(1-10x))
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   ensure_capacity, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/ensure_capacity.h"
#include "../utils/pow2.h"

static seq_t *cache = NULL;
static size_t cache_len   = 0;  /* number of filled terms in cache   */
static size_t cache_cap   = 0;  /* allocated capacity                 */

seq_t a(idx_t n)
{
    /* If already computed, return cached value. */
    if (cache_len > (size_t)n)
        return cache[(size_t)n];

    /* Ensure we have enough space. */
    size_t needed = (size_t)n + 1;
    ensure_capacity(needed, &cache_cap, (void**)&cache, sizeof(seq_t));

    /* Compute missing terms. */
    for (size_t i = cache_len; i <= n; ++i)
    {
        if (i == 0)
        {
            cache[0] = (seq_t)1;
        }
        else if (i == 1)
        {
            cache[1] = (seq_t)21;
        }
        else
        {
            seq_t term = 19 * cache[i-1] - 90 * cache[i-2] + pow2((idx_t)i);
            cache[i] = term;
        }
    }

    cache_len = needed;
    return cache[(size_t)n];
}
