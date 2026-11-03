/*
 * A001894  Number of permutations of {1,...,n} having n-4 inversions (n>=4).
 * https://oeis.org/A001894
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = I_n(n-4), where sum_k I_n(k) x^k = Product_{i=1..n}
 *            (1-x^i)/(1-x). For k <= n, Euler's pentagonal theorem gives
 *            I_n(k) = Sum_{e generalized pentagonal, e<=k} s(e) * C(n-1+k-e,
 *            k-e), with e = j(3j-1)/2 and j(3j+1)/2, sign s(e) = (-1)^j
 *            (s(0)=1). Small n use the product DP directly
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 20, u64 n <= 37, u128 n <= 69,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 887 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t a001894_big(idx_t n) {
    long long k = (long long)(n - 4);
    long long j, cnt = 1, cap, idx, t;
    long long *es;
    int *sg;
    seq_t pos = 0, neg = 0, c = 1;

    for (j = 1; j * (3 * j - 1) / 2 <= k; j++) cnt += 2;
    cap = cnt + 2;
    es = (long long *)malloc((size_t)cap * sizeof(long long));
    sg = (int *)malloc((size_t)cap * sizeof(int));
    cnt = 0;
    es[cnt] = 0; sg[cnt] = 1; cnt++;
    for (j = 1; ; j++) {
        long long u = j * (3 * j - 1) / 2;
        int s = (j & 1) ? -1 : 1;
        if (u > k) break;
        es[cnt] = u; sg[cnt] = s; cnt++;
        if (u + j <= k) { es[cnt] = u + j; sg[cnt] = s; cnt++; }
    }

    idx = cnt - 1;
    for (t = 0; t <= k; t++) {
        /* c = C(n-1+t, t) */
        while (idx >= 0 && k - es[idx] == t) {
            if (sg[idx] > 0) pos += c; else neg += c;
            idx--;
        }
        if (t < k) {
            c = c * (seq_t)(n + (idx_t)t);
            c = c / (seq_t)(t + 1);
        }
    }
    free(es);
    free(sg);
    return pos - neg;
}

static seq_t a001894_small(idx_t n) {
    long long K = (long long)(n - 4);
    seq_t dp[200];
    long long k, i;
    for (k = 0; k <= K; k++) dp[k] = 0;
    dp[0] = 1;
    for (i = 2; i <= (long long)n; i++) {
        for (k = 1; k <= K; k++) dp[k] += dp[k - 1];
        for (k = K; k >= i; k--) dp[k] -= dp[k - i];
    }
    return dp[K];
}

seq_t a(idx_t n) {
    if (n < 4) return 0;
    if (n <= 200) return a001894_small(n);
    return a001894_big(n);
}
