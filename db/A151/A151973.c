/*
 * A151973  Numbers k such that k^2 - k is divisible by 24.
 * https://oeis.org/A151973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-4)-a(n-5) for n>5, with a(1)=0, a(2)=1,
 *            a(3)=9, a(4)=16, a(5)=24
 * Range:     u8 n <= 43, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the initial five terms directly. */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)9;
    if (n == 4) return (seq_t)16;
    if (n == 5) return (seq_t)24;

    /* Static cache of already computed values. */
    static seq_t *cache     = NULL;
    static size_t  cap      = 0; /* number of precomputed terms */

    /* One‑time allocation and initialization. */
    if (!cache) {
        cap   = 5;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* memory failure (unlikely) */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)16;
        cache[4] = (seq_t)24;
    }

    /* Grow the cache if we need to compute more terms. */
    if ((size_t)n > cap) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure -> safe fallback */
        cache = tmp;
        /* Compute all missing terms using the recurrence. */
        for (size_t i = cap; i < new_cap; ++i) {
            /* i corresponds to a(i+1) */
            cache[i] = cache[i-1] + cache[i-4] - cache[i-5];
        }
        cap = new_cap;
    }

    return cache[(size_t)n - 1];
}
