/*
 * A152612  Number of isomorphism classes of n-fold coverings of a connected
 *          graph with Betti number 3.
 * https://oeis.org/A152612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{λ⊢n} z_λ², where z_λ = ∏_i i^{m_i} m_i!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#if defined(__cplusplus)
#include <cstddef>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    idx_t i, j, m, max_m;
    seq_t *dp = NULL;
    seq_t *newdp = NULL;

#if defined(__cplusplus)
    dp = new seq_t[n + 1]();   /* value‑initialise to zero */
    newdp = new seq_t[n + 1]();
#else
    dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    newdp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
#endif

    if (!dp || !newdp) return 0;

    dp[0] = 1;

    for (i = 1; i <= n; ++i) {
        /* zero newdp */
        for (j = 0; j <= n; ++j) newdp[j] = 0;

        max_m = n / i;
#if defined(__cplusplus)
        auto *w = new seq_t[max_m + 1];
#else
        seq_t *w = (seq_t *)calloc((size_t)(max_m + 1), sizeof(seq_t));
#endif
        if (!w) {
#if defined(__cplusplus)
            delete[] dp; delete[] newdp;
#else
            free(dp); free(newdp);
#endif
            return 0;
        }
        w[0] = 1;
        seq_t i2 = (seq_t)(i * i);
        for (m = 1; m <= max_m; ++m) {
            seq_t m_sq = (seq_t)(m * m);
            w[m] = w[m - 1] * i2 * m_sq;
        }

        for (j = 0; j <= n; ++j) {
            if (dp[j] == 0) continue;
            seq_t val = dp[j];
            for (m = 0; j + i * m <= n; ++m) {
                newdp[j + i * m] += val * w[m];
            }
        }

#if defined(__cplusplus)
        delete[] w;
#else
        free(w);
#endif

        /* swap dp and newdp */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
    }

    seq_t result = dp[n];
#if defined(__cplusplus)
    delete[] dp;
    delete[] newdp;
#else
    free(dp);
    free(newdp);
#endif
    return result;
}
