/*
 * A002759  Number of bipartite partitions of n white objects and 10 black
 *          ones.
 * https://oeis.org/A002759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n y^10] Prod_{(i,j) != (0,0)} 1/(1 - x^i y^j) = [x^n]
 *            P(x) * Sum_{partitions of 10 with multiplicities k_j} Prod_j
 *            1/((1-x)(1-x^2)...(1-x^{k_j})), where P(x) = Prod_{i>=1}
 *            1/(1-x^i) (A000041, via pentagonal recurrence)
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 47, u64 n <= 209, u128 n <= 918,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A2759_MAXN 20001

static seq_t a2759_P[A2759_MAXN];
static seq_t a2759_S[A2759_MAXN];
static seq_t a2759_T[A2759_MAXN];
static long long a2759_have = -1;
static int a2759_cnt[11];

static void a2759_add_part(long long N) {
    long long t;
    int j, i;
    for (t = 0; t <= N; t++) a2759_T[t] = 0;
    a2759_T[0] = 1;
    for (j = 1; j <= 10; j++)
        for (i = 1; i <= a2759_cnt[j]; i++)
            for (t = i; t <= N; t++) a2759_T[t] += a2759_T[t - i];
    for (t = 0; t <= N; t++) a2759_S[t] += a2759_T[t];
}

static void a2759_gen(int rem, int maxp, long long N) {
    int p;
    if (rem == 0) { a2759_add_part(N); return; }
    for (p = (rem < maxp ? rem : maxp); p >= 1; p--) {
        a2759_cnt[p]++;
        a2759_gen(rem - p, p, N);
        a2759_cnt[p]--;
    }
}

static void a2759_build(long long N) {
    long long i, k, t;
    for (i = 0; i <= N; i++) {
        if (i == 0) { a2759_P[0] = 1; continue; }
        seq_t pos = 0, neg = 0;
        for (k = 1;; k++) {
            long long g1 = k * (3 * k - 1) / 2;
            long long g2 = k * (3 * k + 1) / 2;
            if (g1 > i) break;
            if (k & 1) pos += a2759_P[i - g1]; else neg += a2759_P[i - g1];
            if (g2 <= i) {
                if (k & 1) pos += a2759_P[i - g2]; else neg += a2759_P[i - g2];
            }
        }
        a2759_P[i] = pos - neg;
    }
    for (t = 0; t <= N; t++) a2759_S[t] = 0;
    for (t = 0; t <= 10; t++) a2759_cnt[t] = 0;
    a2759_gen(10, 10, N);
    a2759_have = N;
}

seq_t a(idx_t n) {
    long long N, k;
    seq_t r = 0;
    if (n >= (idx_t)A2759_MAXN) return 0;
    N = (long long)n;
    if (N > a2759_have) {
        long long target = 2 * a2759_have;
        if (target < N) target = N;
        if (target < 64) target = 64;
        if (target > A2759_MAXN - 1) target = A2759_MAXN - 1;
        a2759_build(target);
    }
    for (k = 0; k <= N; k++) r += a2759_P[k] * a2759_S[N - k];
    return r;
}
