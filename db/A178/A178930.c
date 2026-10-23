/*
 * A178930  Number of semisimple Lie algebras of dimension n.
 * https://oeis.org/A178930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered multisets of simple Lie algebras
 *            (distinct realisations) whose dimensions sum to n. This is
 *            computed with a classic coin‑change dynamic programming over the
 *            list of all simple Lie algebra dimensions up to n
 * Range:     u8 n <= 70, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* OEIS a(0) = 0 */

    /* Prepare list of simple Lie algebra dimensions ≤ n. */
    idx_t dims[200];
    size_t nd = 0;

    /* A_k, k ≥ 1: dim = k^2 + 2k */
    for (idx_t k = 1; ; ++k) {
        idx_t d = k * k + 2 * k;
        if (d > n) break;
        dims[nd++] = d;
    }

    /* B_2 ≅ C_2 has dimension 10 once. */
    if (10 <= n) dims[nd++] = 10;

    /* B_k, k ≥ 3: dim = 2k^2 + k */
    for (idx_t k = 3; ; ++k) {
        idx_t d = 2 * k * k + k;
        if (d > n) break;
        dims[nd++] = d;
    }

    /* C_k, k ≥ 3: dim = 2k^2 + k (distinct from B_k for k ≥ 3) */
    for (idx_t k = 3; ; ++k) {
        idx_t d = 2 * k * k + k;
        if (d > n) break;
        dims[nd++] = d;
    }

    /* D_k, k ≥ 4: dim = 2k^2 - k   (D_3 ≅ A_3 is omitted) */
    for (idx_t k = 4; ; ++k) {
        idx_t d = 2 * k * k - k;
        if (d > n) break;
        dims[nd++] = d;
    }

    /* Exceptional types */
    const idx_t exc[] = {14, 52, 78, 133, 248};
    for (size_t i = 0; i < sizeof(exc) / sizeof(exc[0]); ++i) {
        if (exc[i] <= n) dims[nd++] = exc[i];
    }

    /* Dynamic‑programming table: dp[i] counts partitions summing to i. */
    seq_t *dp = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp) return (seq_t)0;   /* allocation failure → 0 (rare) */

    dp[0] = (seq_t)1;
    for (size_t idx = 0; idx < nd; ++idx) {
        idx_t d = dims[idx];
        for (idx_t i = d; i <= n; ++i) {
            dp[i] += dp[i - d];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
