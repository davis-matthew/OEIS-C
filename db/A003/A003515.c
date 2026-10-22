/*
 * A003515  Number of series-reduced connected labeled graphs with n nodes.
 * https://oeis.org/A003515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   E.g.f. A(x) = log(B(x)), where B(x) is the e.g.f. of A003514
 *            (series-reduced labeled graphs: every node has degree != 2), so
 *            that a(0) = 0 and a(m) = b(m) - Sum_{k=1..m-1}
 *            binomial(m-1,k-1)*a(k)*b(m-k). b(m) is found by
 *            inclusion-exclusion over a set of t marked degree-2 nodes: b(m)
 *            = Sum_{t+s=m}...
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#define S3515_MAXN 400
#define S3515_W (S3515_MAXN + 1)

static seq_t s3515_P[S3515_W * S3515_W];
static seq_t s3515_h[S3515_W + 2];
static seq_t s3515_g[S3515_W + 2];
static seq_t s3515_f[S3515_W + 2];
static seq_t s3515_bp[S3515_W];
static seq_t s3515_bn[S3515_W];
static seq_t s3515_b[S3515_W];
static seq_t s3515_c[S3515_W];
static idx_t s3515_N = 0;
static bool s3515_ready = false;

static void s3515_build(idx_t N) {
    idx_t W = N + 1;
    idx_t i, j, t, s, k, m;

    for (i = 0; i < W; i++)
        for (j = 0; j < W; j++) {
            if (j == 0) s3515_P[i * W + j] = 1;
            else if (j > i) s3515_P[i * W + j] = 0;
            else if (j == i) s3515_P[i * W + j] = 1;
            else s3515_P[i * W + j] = s3515_P[(i - 1) * W + j - 1] + s3515_P[(i - 1) * W + j];
        }

    for (k = 0; k < W + 2; k++) { s3515_h[k] = 0; s3515_g[k] = 0; s3515_f[k] = 0; }
    s3515_h[2] = 1;
    for (k = 3; k < W + 2; k++) s3515_h[k] = s3515_h[k - 1] * (seq_t)k;

    for (m = 0; m < W; m++) { s3515_bp[m] = 0; s3515_bn[m] = 0; s3515_b[m] = 0; s3515_c[m] = 0; }

    {
        seq_t pw = 1, q = 1;
        for (t = 0; t <= N; t++) {
            seq_t T = (seq_t)t;
            idx_t S = N - t;
            if (t > 0) { pw = pw * q; q = q * 2; }
            s3515_g[1] = (seq_t)(t == 0 ? 0 : t * (t - 1) / 2);
            if (S >= 2) s3515_g[2] = T * T;
            for (k = 3; k <= S; k++) s3515_g[k] = s3515_h[k] * T * T + s3515_h[k - 1];
            s3515_f[0] = 1;
            for (s = 1; s <= S; s++) {
                seq_t acc = 0;
                for (k = 1; k <= s; k++)
                    acc += s3515_P[(s - 1) * W + (k - 1)] * s3515_g[k] * s3515_f[s - k];
                s3515_f[s] = acc;
            }
            for (s = 0; s <= S; s++) {
                seq_t term;
                m = s + t;
                term = s3515_P[m * W + s] * pw * s3515_f[s];
                if (s & 1) s3515_bn[m] += term;
                else s3515_bp[m] += term;
            }
        }
    }
    for (m = 0; m <= N; m++) s3515_b[m] = s3515_bp[m] - s3515_bn[m];

    s3515_c[0] = 0;
    for (m = 1; m <= N; m++) {
        seq_t S2 = 0;
        for (k = 1; k < m; k++)
            S2 += s3515_P[(m - 1) * W + (k - 1)] * s3515_c[k] * s3515_b[m - k];
        s3515_c[m] = s3515_b[m] - S2;
    }

    s3515_N = N;
    s3515_ready = true;
}

seq_t a(idx_t n) {
    if (n > S3515_MAXN) return 0;
    if (!s3515_ready || n > s3515_N) {
        idx_t N = n;
        if (s3515_ready && N < s3515_N + s3515_N / 2) N = s3515_N + s3515_N / 2;
        if (N < 16) N = 16;
        if (N > S3515_MAXN) N = S3515_MAXN;
        s3515_build(N);
    }
    return s3515_c[n];
}
