/*
 * A062896  Number of addition triangles with apex n (version 2).
 * https://oeis.org/A062896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of addition triangles with apex n. For each
 *            triangle width k (1 <= k <= floor(log2(n))+1) with binomial
 *            weights w_i = C(k-1, i), the number of bases (positive integers)
 *            b_0 … b_{k-1} satisfying Σ w_i * b_i = n is counted modulo
 *            reversal of the base. The function sums all such counts...
 * Range:     u8 n <= 27, u16 n <= 117, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
/* a(idx_t n) : required entry point */
seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 1; ; ++k) {
        /* minimal sum with all base entries = 1 */
        idx_t S = 1ULL << (k - 1);
        if (S > n) break;
        idx_t N = n - S;          /* target sum for nonnegative y_i */
        /* build binomial coefficients for row k-1 */
        size_t *w = (size_t *)malloc(k * sizeof(size_t));
        if (!w) return total; /* allocation failure – unlikely */
        w[0] = 1;
        for (idx_t i = 1; i < k; ++i)
            w[i] = w[i - 1] * (k - i) / i;

        /* count all solutions for whole weight set (positive bases) */
        size_t Nsize = (size_t)N;
        seq_t *dpC = (seq_t *)calloc(Nsize + 1, sizeof(seq_t));
        if (!dpC) { free(w); return total; }
        dpC[0] = (seq_t)1;
        for (idx_t i = 0; i < k; ++i) {
            size_t wt = w[i];
            for (size_t j = wt; j <= Nsize; ++j)
                dpC[j] += dpC[j - wt];
        }
        seq_t C = dpC[Nsize];
        free(dpC);

        /* count palindromic solutions (reversal invariant bases) */
        seq_t P;
        if (k == 1) {
            P = 1;
        } else {
            size_t half = k / 2;
            size_t palSize = (k % 2 == 0) ? half : half + 1;
            size_t *palW = (size_t *)malloc(palSize * sizeof(size_t));
            if (!palW) { free(w); return total; }

            if (k % 2 == 0) {
                for (size_t i = 0; i < half; ++i) palW[i] = 2 * w[i];
            } else {
                for (size_t i = 0; i < half; ++i) palW[i] = 2 * w[i];
                palW[half] = w[half];            /* middle element */
            }

            seq_t *dpP = (seq_t *)calloc(Nsize + 1, sizeof(seq_t));
            if (!dpP) { free(w); free(palW); return total; }
            dpP[0] = (seq_t)1;
            for (size_t i = 0; i < palSize; ++i) {
                size_t wt = palW[i];
                for (size_t j = wt; j <= Nsize; ++j)
                    dpP[j] += dpP[j - wt];
            }
            P = dpP[Nsize];
            free(dpP);
            free(palW);
        }

        total += (C + P) / 2;
        free(w);
    }
    return total;
}
