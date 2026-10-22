/*
 * A001892  Number of permutations of (1,...,n) having n-2 inversions (n>=2).
 * https://oeis.org/A001892
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   for k = n-2 < n, a(n) = [x^k] Prod_{i=1..n} (1-x^i)/(1-x) =
 *            Sum_{m pentagonal, m<=k} e(m)*C(n-1+k-m, n-1), where e(m) =
 *            (-1)^j for m = j*(3j-1)/2 or j*(3j+1)/2 (Euler's pentagonal
 *            theorem); fixed-width builds use the inversion-count DP T(i,k) =
 *            Sum_{j=0..min(k,i-1)} T(i-1,k-j), T(1,0)=1, which is...
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 36, u128 n <= 68,
 *            big any n
 * Helpers:   a001892_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a001892_big.h"

seq_t a(idx_t n) {
    idx_t k, t, j;
    if (n < 2) return 0;
    k = n - 2;

    if (a001892_big()) {
        signed char *sg = (signed char *)calloc(k + 1, sizeof(signed char));
        seq_t B = 1, pos = 0, neg = 0;
        if (!sg) return 0;
        sg[k] += 1;
        for (j = 1;; j++) {
            idx_t m1 = j * (3 * j - 1) / 2;
            idx_t m2 = j * (3 * j + 1) / 2;
            int s = (j & 1) ? -1 : 1;
            if (m1 > k) break;
            sg[k - m1] += s;
            if (m2 <= k) sg[k - m2] += s;
        }
        for (t = 0; t <= k; t++) {
            if (sg[t] > 0) pos += B;
            else if (sg[t] < 0) neg += B;
            if (t < k) {
                B = B * (seq_t)(n + t);
                B = B / (seq_t)(t + 1);
            }
        }
        free(sg);
        return pos - neg;
    } else {
        seq_t *dp = (seq_t *)calloc(k + 1, sizeof(seq_t));
        seq_t *cum = (seq_t *)calloc(k + 2, sizeof(seq_t));
        seq_t r;
        idx_t i;
        if (!dp || !cum) { free(dp); free(cum); return 0; }
        dp[0] = 1;
        for (i = 2; i <= n; i++) {
            cum[0] = 0;
            for (t = 0; t <= k; t++) cum[t + 1] = cum[t] + dp[t];
            for (t = 0; t <= k; t++) {
                idx_t start = (t + 1 >= i) ? (t + 1 - i) : 0;
                dp[t] = cum[t + 1] - cum[start];
            }
        }
        r = dp[k];
        free(dp);
        free(cum);
        return r;
    }
}
