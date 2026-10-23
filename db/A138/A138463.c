/*
 * A138463  A bisection of A006318.
 * https://oeis.org/A138463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A006318(2n+1) Recurrence (for n ≥ 3): (n+1)*(2n+1)*a(n) =
 *            (70n² – 3n – 4)*a(n‑1) + (‑70n² + 207n – 149)*a(n‑2) +
 *            (n‑2)*(2n‑5)*a(n‑3) Initial values: a(0)=2, a(1)=22, a(2)=394
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are supplied by the build system */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of computed terms */
    static idx_t cache_len = 0;   /* last index computed + 1 */
    static idx_t cache_cap = 0;   /* allocated array capacity */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 4;                   /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;     /* allocation failure */
        /* seed the first three values */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)22;
        cache[2] = (seq_t)394;
        cache_len = 3;
    }

    /* return cached value if already computed */
    if (n < cache_len) return cache[n];

    /* ensure sufficient capacity for indices up to n */
    if (n >= cache_cap) {
        /* double the capacity until it exceeds n */
        while (cache_cap <= n) {
            cache_cap = cache_cap ? cache_cap << 1 : 4;
        }
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;       /* reallocation failed */
        cache = tmp;
    }

    /* compute terms from cache_len up to n using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 3) continue;  /* initial values already set */

        /* Coefficients (non‑negative for i ≥ 3) */
        int64_t c1p = 70LL * (int64_t)i * (int64_t)i - 3LL * (int64_t)i - 4LL;
        int64_t c2p = 70LL * (int64_t)i * (int64_t)i - 207LL * (int64_t)i + 149LL;
        int64_t c3p = (int64_t)(i - 2) * (2LL * (int64_t)i - 5LL);

        /* compute numerator using 128‑bit arithmetic to avoid overflow */
        __uint128_t term1 = (__uint128_t)cache[i-1] * (__uint128_t)c1p;
        __uint128_t term2 = (__uint128_t)cache[i-2] * (__uint128_t)c2p;
        __uint128_t term3 = (__uint128_t)cache[i-3] * (__uint128_t)c3p;
        __uint128_t num   = term1 + term3 - term2;                /* numerator */
        __uint128_t den   = (__uint128_t)(i + 1) * (__uint128_t)(2 * i + 1);

        cache[i] = (seq_t)(num / den);
    }

    cache_len = n + 1;
    return cache[n];
}
