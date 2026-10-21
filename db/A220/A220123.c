/*
 * A220123  Number of tilings of a 4 X n rectangle using integer-sided
 *          rectangular tiles of area 4.
 * https://oeis.org/A220123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 5*a(n-4) - a(n-5) + a(n-6) - a(n-8)
 *            Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=9, a(5)=16,
 *            a(6)=35, a(7)=65
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 63, u128 n <= 125,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_computed = 0;  /* largest index with a known value */

    /* Initialisation: ensure space for the first 8 values and set them. */
    if (cache == NULL) {
        ensure_cache(7, &cache, &cap);  /* at least 8 slots */
        cache[0] = (seq_t)1;             /* a(0) */
        cache[1] = (seq_t)1;             /* a(1) */
        cache[2] = (seq_t)2;             /* a(2) */
        cache[3] = (seq_t)3;             /* a(3) */
        cache[4] = (seq_t)9;             /* a(4) */
        cache[5] = (seq_t)16;            /* a(5) */
        cache[6] = (seq_t)35;            /* a(6) */
        cache[7] = (seq_t)65;            /* a(7) */
        max_computed = 7;
    }

    /* If we already know the requested value, return it. */
    if (n <= max_computed)
        return cache[n];

    /* Ensure we have enough storage for all indices up to n. */
    ensure_cache(n, &cache, &cap);

    /* Compute values incrementally up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i < 8) {      /* safety guard – should not happen after init */
            cache[i] = (seq_t)0;
            continue;
        }
        /* recurrence: a(i) = a(i-1) + a(i-2) + 5*a(i-4) - a(i-5) + a(i-6) - a(i-8) */
        cache[i] = cache[i-1];
        cache[i] += cache[i-2];
        cache[i] += (seq_t)5 * cache[i-4];
        cache[i] -= cache[i-5];
        cache[i] += cache[i-6];
        cache[i] -= cache[i-8];
    }
    max_computed = n;
    return cache[n];
}
