/*
 * A179204  a(1)=11. For n>1, a(n) is LCM of a(n-1) and largest integer <=
 *          sqrt(a(n-1)).
 * https://oeis.org/A179204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 8, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   ensure_capacity, floor_sqrt, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 11; for n > 1, a(n) = lcm(a(n-1), floor_sqrt(a(n-1))) */

#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/floor_sqrt.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL; /* 1‑based cache (index 0 unused) */
    static size_t cache_cap  = 0;     /* allocated capacity (in seq_t elements) */
    static idx_t  cache_max  = 0;     /* highest n already computed */

    if (n == 0) return 0;            /* by definition, never used in OEIS */

    /* Ensure we have room for indices up to n (0..n). */
    ensure_capacity(n + 1, &cache_cap, (void **)&cache, sizeof(seq_t));

    /* Fast return if already computed. */
    if (n <= cache_max) return cache[n];

    /* Initialise the first term on first use. */
    if (cache_max == 0) {
        cache[1] = (seq_t)11;
        cache_max = 1;
    }

    /* Compute missing terms incrementally. */
    for (idx_t i = cache_max + 1; i <= n; ++i)
    {
        seq_t prev = cache[i - 1];
        seq_t sm   = floor_sqrt(prev);
        seq_t g    = gcd(prev, sm);
        seq_t lcm  = prev / g * sm;  /* lcm(prev, sm) */
        cache[i]   = lcm;
    }

    cache_max = n;
    return cache[n];
}
