/*
 * A118096  Number of partitions of n such that the largest part is twice the
 *          smallest part.
 * https://oeis.org/A118096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥1} p_k(n‑3k), where p_k(m) counts partitions of m
 *            whose parts lie in [k,2k]
 * Range:     u8 n <= 50, u16 n <= 157, u32 n <= 521, u64 n <= 1828,
 *            u128 n <= 6727, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    idx_t N = n;
    idx_t max_k = n / 3;

    seq_t *coeff = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!coeff) return 0;

    /* initialize coefficient array to zero */
    for (idx_t i = 0; i <= N; ++i) coeff[i] = 0;
    coeff[0] = 1;

    /* F_1: product_{j=1}^{2} 1/(1-x^j) */
    for (idx_t coin = 1; coin <= 2; ++coin) {
        for (idx_t i = coin; i <= N; ++i) {
            coeff[i] += coeff[i - coin];
        }
    }

    seq_t result = 0;
    /* contribution from k = 1 */
    result += coeff[N - 3];  /* n - 3*1 */

    /* iterate for k = 2 .. max_k */
    for (idx_t k = 2; k <= max_k; ++k) {
        /* step 1: multiply by (1 - x^{k-1}) => subtract */
        idx_t d1 = k - 1;
        for (idx_t i = N; ; --i) {
            coeff[i] -= coeff[i - d1];
            if (i == d1) break;
        }

        /* step 2: multiply by 1/(1 - x^{2k-1}) */
        idx_t d2 = 2 * k - 1;
        if (d2 <= N) {
            for (idx_t i = d2; i <= N; ++i) {
                coeff[i] += coeff[i - d2];
            }
        }

        /* step 3: multiply by 1/(1 - x^{2k}) */
        idx_t d3 = 2 * k;
        if (d3 <= N) {
            for (idx_t i = d3; i <= N; ++i) {
                coeff[i] += coeff[i - d3];
            }
        }

        /* contribution from current k */
        result += coeff[N - 3 * k];
    }

    free(coeff);
    return result;
}
