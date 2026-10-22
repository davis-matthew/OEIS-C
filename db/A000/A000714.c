/*
 * A000714  Number of partitions of n, with three kinds of 1 and 2 and two
 *          kinds of 3,4,5,....
 * https://oeis.org/A000714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. = P(x) * B(x), where P(x) = 1/Product_{k>=1}(1-x^k)
 *            (A000041, via the pentagonal number recurrence p(m) = Sum_{k>=1}
 *            (-1)^(k+1) (p(m-k(3k-1)/2) + p(m-k(3k+1)/2)), p(0)=1) and B(x) =
 *            1/((1-x)*(1-x^2)*Product_{k>=1}(1-x^k)), computed from the
 *            sparse series D(x) = (1-x)(1-x^2)*Sum_k (-1)^k x^(k...
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 55, u64 n <= 186, u128 n <= 676,
 *            big any n
 * Helpers:   alloc_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"

static seq_t *P_ = 0;
static seq_t *B_ = 0;
static idx_t cap_ = 0;

static void build(idx_t N) {
    seq_t *P = alloc_zero(N);
    seq_t *B = alloc_zero(N);
    int *d = (int *)calloc((size_t)N + 2, sizeof(int));
    idx_t *J = (idx_t *)malloc(((size_t)N + 2) * sizeof(idx_t));
    int *C = (int *)malloc(((size_t)N + 2) * sizeof(int));
    idx_t cnt = 0, i, m;
    unsigned long long k;

    /* pentagonal series E(x) */
    d[0] = 1;
    for (k = 1;; k++) {
        unsigned long long g1 = k * (3 * k - 1) / 2;
        int s = (k & 1) ? -1 : 1;
        if (g1 >= N) break;
        d[g1] += s;
        if (g1 + k < N) d[g1 + k] += s;
    }
    /* multiply by (1-x) */
    for (i = N - 1; i >= 1; i--) d[i] -= d[i - 1];
    /* multiply by (1-x^2) */
    for (i = N - 1; i >= 2; i--) d[i] -= d[i - 2];
    for (i = 1; i < N; i++) {
        if (d[i] != 0) { J[cnt] = i; C[cnt] = -d[i]; cnt++; }
    }

    P[0] = 1;
    for (m = 1; m < N; m++) {
        seq_t pos = 0, neg = 0;
        for (k = 1;; k++) {
            unsigned long long g1 = k * (3 * k - 1) / 2;
            unsigned long long g2 = g1 + k;
            if (g1 > m) break;
            if (k & 1) {
                pos += P[m - g1];
                if (g2 <= m) pos += P[m - g2];
            } else {
                neg += P[m - g1];
                if (g2 <= m) neg += P[m - g2];
            }
        }
        P[m] = pos - neg;
    }

    B[0] = 1;
    for (m = 1; m < N; m++) {
        seq_t pos = 0, neg = 0;
        idx_t t;
        for (t = 0; t < cnt && J[t] <= m; t++) {
            int c = C[t];
            if (c > 0) pos += (seq_t)c * B[m - J[t]];
            else neg += (seq_t)(-c) * B[m - J[t]];
        }
        B[m] = pos - neg;
    }

    free(d);
    free(J);
    free(C);
    P_ = P;
    B_ = B;
    cap_ = N;
}

seq_t a(idx_t n) {
    idx_t i;
    seq_t s = 0;
    if (n >= cap_) {
        idx_t N = n + 1;
        if (N < 2 * cap_) N = 2 * cap_;
        if (N < 64) N = 64;
        build(N);
    }
    for (i = 0; i <= n; i++) s += P_[i] * B_[n - i];
    return s;
}
