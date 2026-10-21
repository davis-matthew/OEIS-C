/*
 * A052365  Number of nonnegative integer 3 X 3 matrices with sum of elements
 *          equal to n, under row and column permutations.
 * https://oeis.org/A052365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of x^n in
 *            -(x^10+2x^8+x^7+7x^6-3x^5+4x^3+x^2-2x+1) /
 *            ((x^4-x^3+x-1)*(x^3-1)^3*(x+1)^3*(x-1)^5) The sequence satisfies
 *            a linear recurrence of order 21 derived from the denominator
 *            polynomial
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 89, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n) {
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* first 21 terms (indices 0..20) */
    static const seq_t seed[21] = {
        1, 1, 4, 10, 24, 51, 114, 219, 424, 768,
        1352, 2278, 3759, 5978, 9328, 14181, 21164,
        30943, 44560, 63063, 88088
    };

    /* initialize cache */
    if (cache == NULL) {
        cache_cap = 21;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 21; ++i) cache[i] = seed[i];
    }
    /* grow cache if necessary */
    if ((idx_t)n >= (idx_t)cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n) new_cap = n + 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (cache == NULL) { /* malloc failure */
            abort();
        }
        cache_cap = new_cap;
    }
    if (n < 21) return cache[n];

    /* recurrence coefficients derived from the denominator polynomial */
    static int64_t coef[22];
    static int64_t coef_deg = 0;
    if (coef_deg == 0) {
        /* Polynomial A = x^4 - x^3 + x - 1 */
        const int64_t A[5] = { -1, 1, 0, -1, 1 };  /* indices 0..4 */

        /* Polynomial B1 = x^3 - 1 */
        const int64_t B1[4] = { -1, 0, 0, 1 };      /* indices 0..3 */

        /* Compute B = B1^3 */
        int64_t tmp1[7] = {0};
        for (int i = 0; i <= 3; ++i)
            for (int j = 0; j <= 3; ++j)
                tmp1[i + j] += B1[i] * B1[j];

        int64_t B[10] = {0};
        for (int i = 0; i <= 6; ++i)
            for (int j = 0; j <= 3; ++j)
                B[i + j] += tmp1[i] * B1[j];

        /* B now has degree 9 (indices 0..9) */

        /* Polynomial C = (x+1)^3 = x^3 + 3x^2 + 3x + 1 */
        const int64_t C[4] = { 1, 3, 3, 1 };  /* indices 0..3 */

        /* Multiply A and B -> AB (degree 13) */
        int64_t AB[14] = {0};
        for (int i = 0; i <= 4; ++i)
            for (int j = 0; j <= 9; ++j)
                AB[i + j] += A[i] * B[j];

        /* Multiply AB with C -> AC (degree 16) */
        int64_t AC[17] = {0};
        for (int i = 0; i <= 13; ++i)
            for (int j = 0; j <= 3; ++j)
                AC[i + j] += AB[i] * C[j];

        /* Polynomial Dp = (x-1)^5 */
        const int64_t Dp[6] = { -1, 5, -10, 10, -5, 1 }; /* indices 0..5 */

        /* Multiply AC with Dp -> Denominator D (degree 21) */
        int64_t D[22] = {0};
        for (int i = 0; i <= 16; ++i)
            for (int j = 0; j <= 5; ++j)
                D[i + j] += AC[i] * Dp[j];

        /* Store coefficients (deg = 21) */
        coef_deg = 21;
        for (int i = 0; i <= 21; ++i) coef[i] = D[i];
    }

    /* Ensure cache filled up to n */
    for (idx_t i = 21; i <= n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (int k = 1; k <= coef_deg; ++k) {
            seq_t term = cache[i - k];
            if (coef[k] >= 0)
                pos += mul_small(term, (unsigned int)coef[k]);
            else
                neg += mul_small(term, (unsigned int)(-coef[k]));
        }
        cache[i] = pos - neg;  /* pos >= neg */
    }
    return cache[n];
}
