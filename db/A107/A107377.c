/*
 * A107377  Expansion of x*(1-4*x-3*x^2)/(1-5*x+5*x^3+x^4).
 * https://oeis.org/A107377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 5*a(n-3) - a(n-4); a(0)=0, a(1)=1, a(2)=1,
 *            a(3)=2
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t last_computed = 3;

    /* Initialise cache with the first four terms */
    if (cache == NULL) {
        cache_size = 4;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;          /* out‑of‑memory fallback */
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 2;
    }

    if (n <= 3) return cache[n];

    /* Expand cache if necessary */
    if ((idx_t)(cache_size) <= n) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;          /* out‑of‑memory fallback */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i] = 5 * cache[i-1] - 5 * cache[i-3] - cache[i-4];
    }
    last_computed = n;

    return cache[n];
}
