/*
 * A058916  Erroneous version of A001500.
 * https://oeis.org/A058916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = A001500(n) for n != 3, a(3) = 5 (erroneous term).
 *            A001500(n) is the number of n X n nonnegative integer matrices
 *            with all row and column sums equal to 3. For n <= 16 it is
 *            counted by a row-by-row DP over the column states (c1,c2,c3) =
 *            number of columns with remaining capacity 1,2,3. For la...
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 6, i64 n <= 8, i128 n <= 8,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 8 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t A001500_big(idx_t n) {
    seq_t N = 0;
    seq_t W = 1;      /* C(n,g) * n!/g! * 6^(n-g) */
    seq_t F = 1;      /* (3g)! */
    seq_t P2 = 1;     /* 2^(n-g) */
    idx_t i, g;
    for (i = 1; i <= n; i++) W *= (idx_t)i;
    for (i = 0; i < n; i++) { W *= 6; P2 *= 2; }
    for (g = 0; g <= n; g++) {
        idx_t m = n - g;
        seq_t t = P2 * F;
        seq_t I = t;
        idx_t b;
        for (b = 0; b < m; b++) {
            t *= (idx_t)(3 * (m - b) * (3 * g + b + 1));
            t /= (idx_t)(2 * (b + 1));
            I += t;
        }
        N += W * I;
        if (g < n) {
            W *= (idx_t)(n - g);
            W /= (idx_t)(6 * (g + 1) * (g + 1));
            F *= (idx_t)((3 * g + 1) * (3 * g + 2) * (3 * g + 3));
            P2 /= 2;
        }
    }
    for (i = 0; i < n; i++) N /= 36;
    return N;
}

static long long A001500_sb(long long c, int k) {
    if (k == 0) return 1;
    if (c < k) return 0;
    if (k == 1) return c;
    if (k == 2) return c * (c - 1) / 2;
    return c * (c - 1) * (c - 2) / 6;
}

static seq_t A001500_dp(idx_t n) {
    long long N1 = (long long)n + 1;
    long long sz = N1 * N1 * N1;
    seq_t *dp = mem_new(seq_t, (size_t)sz);
    seq_t *nd = mem_new(seq_t, (size_t)sz);
    long long i, r;
    for (i = 0; i < sz; i++) dp[i] = 0;
    dp[((long long)n * N1 + 0) * N1 + 0] = 1;
    for (r = 0; r < (long long)n; r++) {
        long long c3, c2, c1;
        for (i = 0; i < sz; i++) nd[i] = 0;
        for (c3 = 0; c3 <= (long long)n; c3++)
        for (c2 = 0; c2 + c3 <= (long long)n; c2++)
        for (c1 = 0; c1 + c2 + c3 <= (long long)n; c1++) {
            seq_t v = dp[(c3 * N1 + c2) * N1 + c1];
            long long c[4];
            long long d[4];
            int k, j, e1, e2;
            if (v == 0) continue;
            c[0] = 0; c[1] = c1; c[2] = c2; c[3] = c3;
            /* one column gets 3 */
            if (c3 > 0) {
                nd[((c3 - 1) * N1 + c2) * N1 + c1] += v * c3;
            }
            /* one column gets 2, a different one gets 1 */
            for (k = 2; k <= 3; k++) {
                for (j = 1; j <= 3; j++) {
                    long long cj = c[j] - (j == k ? 1 : 0);
                    long long w;
                    if (c[k] <= 0 || cj <= 0) continue;
                    w = c[k] * cj;
                    d[0] = 0; d[1] = c[1]; d[2] = c[2]; d[3] = c[3];
                    d[k]--; d[k - 2]++;
                    d[j]--; d[j - 1]++;
                    nd[(d[3] * N1 + d[2]) * N1 + d[1]] += v * w;
                }
            }
            /* three distinct columns get 1 each */
            for (e1 = 0; e1 <= 3; e1++) {
                for (e2 = 0; e1 + e2 <= 3; e2++) {
                    int e3 = 3 - e1 - e2;
                    long long w = A001500_sb(c[1], e1) *
                                  A001500_sb(c[2], e2) *
                                  A001500_sb(c[3], e3);
                    if (w == 0) continue;
                    d[1] = c[1] - e1 + e2;
                    d[2] = c[2] - e2 + e3;
                    d[3] = c[3] - e3;
                    nd[(d[3] * N1 + d[2]) * N1 + d[1]] += v * w;
                }
            }
        }
        {
            seq_t *tmp = dp; dp = nd; nd = tmp;
        }
    }
    {
        seq_t res = dp[0];
        mem_free(dp);
        mem_free(nd);
        return res;
    }
}

static seq_t A001500_a(idx_t n) {
    /* DP keeps intermediates bounded (exact in any width where a(n) fits;
       int128 holds up to n = 16). Beyond that only bignum builds matter. */
    if (n <= 16) return A001500_dp(n);
    return A001500_big(n);
}

seq_t a(idx_t n) {
    if (n == 3) return 5;   /* the erroneous term of this dead entry */
    return A001500_a(n);
}
