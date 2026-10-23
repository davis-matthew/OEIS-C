/*
 * A165169  Number of reduced words of length n in Coxeter group on 37
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^8 = I.
 * https://oeis.org/A165169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Let p(n) be the coefficient of t^n in the numerator P(t) = 1 +
 *            2t + 2t^2 + … + 2t^7 + t^8, i.e. p(0)=1, p(1..7)=2, p(8)=1,
 *            p(n)=0 for n>8. Then the coefficients a(n) of G(t) = P(t) / (1 −
 *            35t − 35t^2 − … − 35t^7 + 630t^8) satisfy a(n) =
 *            35·(a(n-1)+…+a(n-7)) – 630·a(n-8) + p(n), with a(0)=1. This...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached values of the sequence. */
    static seq_t *cache = NULL;          /* cache array                      */
    static size_t cache_cap = 0;         /* allocated capacity (elements)    */
    static idx_t cache_max = 0;          /* highest n that has been computed */

    /* Coefficients of the numerator: p(0)=1, p(1..7)=2, p(8)=1. */
    static const seq_t P[9] = {1, 2, 2, 2, 2, 2, 2, 2, 1};

    /* Initialize cache on first call. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failed */
        cache[0] = (seq_t)1;                  /* a(0) */
        cache_max = 0;
    }

    /* Grow cache if necessary. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return cached value if already computed. */
    if (n <= cache_max) return cache[n];

    /* Compute missing terms up to n. */
    for (idx_t k = cache_max + 1; k <= n; ++k) {
        seq_t sum = (seq_t)0;

        /* 35*(a(k-1)+…+a(k-7)), but only terms with non‑negative index. */
        if (k >= 1) {
            idx_t maxj = (k < 7) ? k : 7;
            for (idx_t j = 1; j <= maxj; ++j)
                sum += (seq_t)35 * cache[k - j];
        }

        /* Subtract 630*a(k-8) when the index is valid. */
        if (k >= 8)
            sum -= (seq_t)630 * cache[k - 8];

        /* Add the numerator coefficient p(k). */
        if (k <= 8)      /* p(k) is non‑zero only for k ≤ 8 */
            sum += P[(size_t)k];

        cache[k] = sum;
    }

    cache_max = n;
    return cache[n];
}
