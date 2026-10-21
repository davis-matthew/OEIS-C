/*
 * A061794  Number of distinct sums d(i) + d(j) for 1<=i<=j<=n, d(k) =
 *          A000005(k) = number of divisors function.
 * https://oeis.org/A061794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct sums τ(i)+τ(j) for 1 ≤ i ≤ j ≤ n,
 *            where τ(k) is the number of divisors of k (A000005)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* --- compute τ[1..n] ------------------------------------------- */
    /* τ values are small for n ≤ 10000, so uint16_t suffices.         */
    uint16_t *tau = (uint16_t*)malloc((n + 1) * sizeof(uint16_t));
    if (!tau) return (seq_t)0;             /* allocation failure */

    /* initialize to 0: tau[0] is ignored */
    for (idx_t i = 1; i <= n; ++i) tau[i] = 0;

    for (idx_t d = 1; d <= n; ++d) {
        for (idx_t m = d; m <= n; m += d) {
            ++tau[m];
        }
    }

    /* --- find maximum τ value -------------------------------------- */
    uint16_t maxTau = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (tau[i] > maxTau) maxTau = tau[i];
    }

    /* --- count frequency of each τ value --------------------------- */
    uint16_t *freq = (uint16_t*)calloc((size_t)maxTau + 1, sizeof(uint16_t));
    if (!freq) {
        free(tau); return (seq_t)0;
    }
    for (idx_t i = 1; i <= n; ++i) {
        ++freq[tau[i]];
    }

    /* --- mark attainable sums τ(t1)+τ(t2) with t1 ≤ t2 -------------- */
    size_t sumSize = (size_t)(maxTau * 2 + 1);
    bool *seen = (bool*)calloc(sumSize, sizeof(bool));
    if (!seen) {
        free(tau); free(freq); return (seq_t)0;
    }

    seq_t distinct = (seq_t)0;
    for (uint16_t t1 = 1; t1 <= maxTau; ++t1) {
        if (freq[t1] == 0) continue;
        for (uint16_t t2 = t1; t2 <= maxTau; ++t2) {
            if (freq[t2] == 0) continue;
            size_t s = (size_t)t1 + (size_t)t2;
            if (!seen[s]) {
                seen[s] = true;
                ++distinct;
            }
        }
    }

    /* --- cleanup ---------------------------------------------------- */
    free(tau); free(freq); free(seen);

    return distinct;
}
