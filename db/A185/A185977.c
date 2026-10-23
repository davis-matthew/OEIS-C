/*
 * A185977  Minimal number of parts of multiset repetition class defining
 *          partitions of n.
 * https://oeis.org/A185977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = minimal number of parts of a multiset repetition * class
 *            defining a partition of n. * * For each possible maximal part M
 *            (1 ≤ M ≤ Mmax(n) where * Mmax(n) is the largest integers such
 *            that M(M+1)/2 ≤ n) we * consider the representation * n =
 *            M(M+1)/2 + Σk·t_k·(k(k+1)/2 ) (0 ≤ k ≤ M) * * The...
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

seq_t a(idx_t n)
{
    /* Undefined for n = 0 – return 0, but OEIS offset starts at 1. */
    if (n == 0) return 0;

    /* Largest M such that M(M+1)/2 ≤ n (Mmax(n)). */
    idx_t Mmax = 0;
    for (idx_t m = 1; ; ++m) {
        seq_t tri = (seq_t)m * (m + 1) / 2;
        if (tri > (seq_t)n) break;
        Mmax = m;
    }

    seq_t best = (seq_t)n + (seq_t)Mmax;   /* crude upper bound */

    for (idx_t M = 1; M <= Mmax; ++M) {
        seq_t tri = (seq_t)M * (M + 1) / 2;
        idx_t r = n - (idx_t)tri;          /* remainder to be filled */

        seq_t cost = 0;                    /* extra parts beyond M */

        if (r > 0) {
            /* Unbounded coin change DP: minimal parts to achieve weight r. */
            const int INF = INT_MAX / 4;
            int *dp = (int*)malloc((size_t)(r + 1) * sizeof(int));
            if (!dp) return 0; /* allocation failed – fail gracefully */
            for (idx_t i = 0; i <= r; ++i) dp[i] = INF;
            dp[0] = 0;

            for (idx_t k = 1; k <= M; ++k) {
                seq_t w_seq = (seq_t)k * (k + 1) / 2;
                if (w_seq > (seq_t)r) break;
                idx_t w = (idx_t)w_seq;
                int add = (int)k;
                for (idx_t v = w; v <= r; ++v) {
                    int prev = dp[v - w];
                    if (prev + add < dp[v]) dp[v] = prev + add;
                }
            }
            cost = dp[r];
            free(dp);
        }

        seq_t total = (seq_t)M + cost;
        if (total < best) best = total;
    }

    return best;
}
