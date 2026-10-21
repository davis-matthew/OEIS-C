/*
 * A088980  G.f.: 1 + x + Sum_{n >= 1} ( x^(2^n)/
 *          ((1+x^(2^(n-1)))*Product_{j=0..n-1} (1-x^(2^j)) ) ).
 * https://oeis.org/A088980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of x^n in 1 + x + Σ_{k≥1} x^(2^k)/((1 +
 *            x^(2^(k-1)))·∏_{j=0}^{k-1} (1 - x^(2^j)))
 * Range:     u8 n <= 51, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* Work only up to the required index n */
    size_t N = (size_t)n;

    /* Allocate array for coefficients a[0..N] */
    seq_t *coeff = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!coeff) return (seq_t)0;   /* allocation failed */

    coeff[0] = (seq_t)1;
    coeff[1] = (seq_t)1;

    /* Find the largest power of 2 not exceeding N */
    size_t max_k = 0;
    while (((size_t)1ULL << max_k) <= N) ++max_k;

    /* Process each term k = 1 .. max_k of the generating function */
    for (size_t k = 1; k <= max_k; ++k)
    {
        if (k == 1)
        {
            /* H_1(x) = x^2/(1 - x^2) = Σ_{t≥1} x^(2t) */
            for (size_t shift = 2; shift <= N; shift += 2)
                coeff[shift] += (seq_t)1;
        }
        else
        {
            /* Build the coefficient array for C_k(x) = 1/∏_{j=0}^{k-2}(1 - x^(2^j)) */
            seq_t *C = (seq_t *)calloc(N + 1, sizeof(seq_t));
            if (!C) { free(coeff); return (seq_t)0; }
            C[0] = (seq_t)1;

            for (size_t j = 0; j <= k - 2; ++j)
            {
                size_t d = 1ULL << j;         /* d = 2^j */
                for (size_t e = d; e <= N; ++e)
                    C[e] += C[e - d];
            }

            /* Add contributions of H_k(x) = Σ_{t≥1} x^(t·2^k) · C(x) */
            size_t shift_base = 1ULL << k;    /* 2^k */
            for (size_t shift = shift_base; shift <= N; shift += shift_base)
            {
                size_t limit = N - shift;
                for (size_t exp = 0; exp <= limit; ++exp)
                    coeff[shift + exp] += C[exp];
            }

            free(C);
        }
    }

    seq_t result = coeff[N];
    free(coeff);
    return result;
}
