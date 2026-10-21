/*
 * A089138  a(n) = (3^(2*n))*(integral_{x=0 to 1} (1+x^3)^n dx)/(integral_{x=0
 *          to 1} (1-x^3)^n dx).
 * https://oeis.org/A089138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3^(2*n)) * ∫₀¹ (1+x^3)^n dx / ∫₀¹ (1−x^3)^n dx
 *            Recurrence: n*a(n) = 3*(9*n−4)*a(n−1) − 54*(3*n−2)*a(n−2)
 *            a(0)=1, a(1)=15
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* array of computed terms */
    static size_t cache_cap  = 0;      /* allocated capacity in entries */
    static size_t cache_len  = 0;      /* number of initialized entries */

    /* Ensure sufficient capacity */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        if (cache) {
            for (size_t i = 0; i < cache_len; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialise first two terms */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)15;  /* a(1) */
        cache_len = 2;
    }

    /* Return cached value if already computed */
    if ((size_t)n < cache_len)
        return cache[n];

    /* Compute missing terms using recurrence */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
#ifdef __cplusplus
        /* C++ mode – seq_t is an arbitrary‑precision class, use it directly */
        seq_t N      = (seq_t)i;
        seq_t term1  = 9 * N - 4;          /* 9*n - 4 */
        seq_t term2  = 3 * N - 2;          /* 3*n - 2 */
        seq_t num    = 3 * term1 * cache[i-1]
                     - 54 * term2 * cache[i-2];
        cache[i] = num / N;
#else
        /* C mode – use 128‑bit intermediate to avoid overflow (for n ≤ 16) */
        __uint128_t N = i;
        __uint128_t term1 = 9 * N - 4;
        __uint128_t term2 = 3 * N - 2;
        __uint128_t num = 3 * term1 * ( __uint128_t )cache[i-1]
                         - 54 * term2 * ( __uint128_t )cache[i-2];
        cache[i] = (seq_t)(num / N);
#endif
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
