/*
 * A232665  Number of compositions of 2n such that the largest multiplicity of
 *          parts equals n.
 * https://oeis.org/A232665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1. For n ≥ 1, a(n) = 1 + ∑_{k=1}^{⌊n/2⌋} C(n + k, n) ·
 *            C(n − k − 1, k − 1) where C(a, b) is the binomial coefficient a
 *            choose b
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 38, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiplicative binomial coefficient: 0 ≤ K ≤ N */
static seq_t binom_multiplicative(idx_t N, idx_t K)
{
    if (K > N - K) K = N - K;          /* use the smaller argument */
    seq_t result = 1;
    for (idx_t i = 1; i <= K; ++i) {
        result = result * (seq_t)(N - K + i);
        result = result / (seq_t)i;     /* division is exact at each step */
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t sum = 1;                     /* the composition [2,2,…,2] */
    idx_t half = n / 2;                /* maximum k such that 2k ≤ n */

    for (idx_t k = 1; k <= half; ++k) {
        seq_t c1 = binom_multiplicative(n + k, n);    /* C(n + k, n) */
        seq_t c2 = binom_multiplicative(n - k - 1, k - 1); /* C(n−k−1, k−1) */
        sum = sum + c1 * c2;
    }

    return sum;
}
