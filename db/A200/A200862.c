/*
 * A200862  G.f.: (1-2*x^2)/(1-2*x-5*x^2+9*x^3).
 * https://oeis.org/A200862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=2, a(2)=7, a(n)=2*a(n-1)+5*a(n-2)-9*a(n-3) for n>2
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute sequence A200862.
 * The function uses a simple memoisation cache which grows as needed.
 * It is safe for repeated calls with arbitrary indices. */
seq_t a(idx_t n)
{
    /* Base cases for small n, handled directly below if cache has enough data */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* capacity allocated for cache */
    static size_t cache_len = 0;   /* elements already filled: indices 0..cache_len-1 */

    /* Ensure the cache can hold (n+1) elements */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – fall back to direct computation */
            seq_t a0 = 1, a1 = 2, a2 = 7;
            if (n == 0) return a0;
            if (n == 1) return a1;
            if (n == 2) return a2;
            for (size_t i = 3; i <= n; ++i) {
                seq_t an = 2 * a2 + 5 * a1 - 9 * a0;
                a0 = a1; a1 = a2; a2 = an;
            }
            return a2;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If we already have the answer, return it immediately */
    if (cache_len > n) return cache[n];

    /* Initialise base values if not yet stored */
    if (cache_len < 3) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)7;
        cache_len = 3;
    }

    /* Compute from the first missing index up to n */
    for (size_t i = cache_len; i <= n; ++i) {
        cache[i] = 2 * cache[i-1] + 5 * cache[i-2] - 9 * cache[i-3];
    }
    cache_len = n + 1;

    return cache[n];
}
