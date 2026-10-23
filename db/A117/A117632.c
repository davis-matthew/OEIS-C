/*
 * A117632  Number of 1's required to build n using {+,T} and parentheses,
 *          where T(i) = i*(i+1)/2.
 * https://oeis.org/A117632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 * a(1) = 1.
 * For n > 1:
 *   a(n) = min( a(k) + a(n - k)   for 1 ≤ k ≤ n-1,
 *              a(k)                if n = k*(k+1)/2 ) */

#include <stdlib.h>

/* integer square root of an unsigned 64‑bit value, using 128‑bit multiplication to avoid overflow */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = (1ULL << 32);          /* sqrt(2^64-1) < 2^32   */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* cache[0]..cache[cache_cap-1] */
    static size_t cache_cap = 0;   /* number of entries stored */

    if (n < 1)                     /* offset starts at 1 */
        return 0;

    /* grow the cache if needed */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();         /* allocation failure */
        cache = tmp;

        /* initialise entries that have not been computed yet */
        for (idx_t i = cache_cap; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 0;
                continue;
            }
            if (i == 1) {
                cache[1] = 1;
                continue;
            }

            /* Start with a trivial representation: 1 + (i-1) */
            seq_t best = cache[1] + cache[i - 1];

            /* Try all partitions i = k + (i-k) */
            for (idx_t k = 1; k <= i / 2; ++k) {
                seq_t cost = cache[k] + cache[i - k];
                if (cost < best) best = cost;
            }

            /* Check if i is a triangular number T(k) = k*(k+1)/2 */
            idx_t D = 1 + 8 * i;
            idx_t s = isqrt_u64(D);
            if (s * s == D) {
                idx_t k = (s - 1) / 2;
                if (k > 0) {
                    /* Verify to avoid overflow */
                    idx_t t = (idx_t)(((unsigned __int128)k * (k + 1)) / 2);
                    if (t == i) {
                        seq_t cost = cache[k];
                        if (cost < best) best = cost;
                    }
                }
            }

            cache[i] = best;
        }

        cache_cap = new_cap;
    }

    return cache[n];
}
