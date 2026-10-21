/*
 * A052962  Expansion of (1-2x^2)/(1-x-3x^2+2x^4).
 * https://oeis.org/A052962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=2, a(3)=5, a(n+4)=3*a(n+2)+a(n+3)-2*a(n)
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 57, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for OEIS A052962.  The sequence satisfies
 * a(n+4) = 3*a(n+2) + a(n+3) - 2*a(n) (n ≥ 0) with the base
 * values a(0)=1, a(1)=1, a(2)=2, a(3)=5.
 * This implementation memoises computed terms so that any ordering of
 * calls is fast and works for arbitrary‑precision seq_t or any
 * unsigned integer size. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL; /* array of computed terms */
    static size_t cap   = 0;    /* allocated capacity */
    static size_t len   = 0;    /* number of initialised elements (next free index) */

    /* initialise cache with base terms */
    if (cap == 0) {
        cap   = 4;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[0] = 1; /* a(0) */
        cache[1] = 1; /* a(1) */
        cache[2] = 2; /* a(2) */
        cache[3] = 5; /* a(3) */
        len   = 4;    /* next free index */
    }

    /* grow the cache if n is outside the current capacity */
    if (n >= cap) {
        size_t newcap = cap * 2;
        while (newcap <= n) newcap *= 2;
        if (newcap < 4) newcap = 4;
        cache = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        cap   = newcap;
    }

    /* compute missing terms up to n (if any) */
    if (n >= len) {
        for (idx_t i = len; i <= n; ++i) {
            if (i <= 3) { /* already initialised */
                continue;
            }
            /* a(i) = 3*a(i-2) + a(i-1) - 2*a(i-4) */
            seq_t three_a_i2 = cache[i-2] + cache[i-2] + cache[i-2];
            seq_t two_a_i4   = cache[i-4] + cache[i-4];
            cache[i] = three_a_i2 + cache[i-1] - two_a_i4;
        }
        len = n + 1;
    }

    return cache[n];
}
