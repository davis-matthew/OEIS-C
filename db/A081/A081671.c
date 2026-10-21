/*
 * A081671  Expansion of e.g.f. exp(4x) * I_0(2x).
 * https://oeis.org/A081671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Recurrence:  n*a(n) = 4*(2*n-1)*a(n-1) - 12*(n-1)*a(n-2)
   with the initial values a(0)=1, a(1)=4. */
/* Formula in OEIS notation: a(n) = Sum_{k=0..n} 2^(n-k) * C(n,k) * C(2k,k) */

seq_t a(idx_t n)
{
    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Initialise cache with the first two terms. */
    if (cache_len == 0) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache_len = 2;
    }

    /* Grow the cache if the requested index is beyond the current capacity. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 2;
        while (new_cap <= (size_t)n + 1)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute all missing terms up to the requested index. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i < 2)        /* a(0) and a(1) already set */
            continue;

        if (sizeof(seq_t) <= 8) { /* uint64_t or smaller: use 128‑bit intermediates */
            long long ln = (long long)i;
            __int128 num1 = (__int128)cache[i - 1];
            num1 *= 4;
            num1 *= (2 * ln - 1);

            __int128 num2 = (__int128)cache[i - 2];
            num2 *= 12;
            num2 *= (ln - 1);

            __int128 numer = num1 - num2;
            cache[i] = (seq_t)(numer / i);
        } else { /* seq_t is arbitrary‑precision or 128‑bit: stay in seq_t arithmetic */
            seq_t part1 = cache[i - 1] * (seq_t)(4 * (2 * i - 1));
            seq_t part2 = cache[i - 2] * (seq_t)(12 * (i - 1));
            cache[i] = (part1 - part2) / (seq_t)i;
        }
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
