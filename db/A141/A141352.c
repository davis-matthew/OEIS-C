/*
 * A141352  Expansion of (1-2x-5x^2-7x^3+x^6)/((1-x)(1-x^3)^2).
 * https://oeis.org/A141352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) + 2*a(n-3) - 2*a(n-4) - a(n-6) + a(n-7) for n ≥ 7,
 *            with initial values a(0)=1, a(1)=-1, a(2)=-6, a(3)=-11,
 *            a(4)=-15, a(5)=-25, a(6)=-35
 * Range:     i8 n <= 12, i16 n <= 40, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;     /* capacity of the cache array */
    static size_t cache_nmax = 0;    /* number of terms currently stored (max index + 1) */
    static int initialized = 0;      /* flag to perform one‑time initialization */

    /* one‑time initialization: allocate cache and set first 7 terms */
    if (!initialized) {
        cache_cap = 8; /* at least enough for indices 0…6 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed – return 0 as a safe fallback */
        cache[0] =  1;
        cache[1] = -1;
        cache[2] = -6;
        cache[3] = -11;
        cache[4] = -15;
        cache[5] = -25;
        cache[6] = -35;
        cache_nmax = 7;            /* we have stored indices 0…6 */
        initialized = 1;
    }

    /* request higher n: extend the cache if necessary */
    if (n >= cache_nmax) {
        size_t need = (size_t)(n + 1);
        if (need > cache_cap) {
            /* grow the cache – double until it fits */
            size_t new_cap = cache_cap ? cache_cap : 8;
            while (new_cap < need) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0; /* reallocation failed – return 0 as fallback */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute terms from cache_nmax up to n (inclusive) */
        size_t i;
        for (i = cache_nmax; i <= n; ++i) {
            /* recurrence is valid for i ≥ 7 */
            cache[i] = cache[i-1] + 2*cache[i-3] - 2*cache[i-4] - cache[i-6] + cache[i-7];
        }
        cache_nmax = n + 1;
    }

    /* return the requested value */
    return cache[(size_t)n];
}
