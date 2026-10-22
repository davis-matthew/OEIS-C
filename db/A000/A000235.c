/*
 * A000235  Number of n-node rooted trees of height 3.
 * https://oeis.org/A000235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001383(n) - A000041(n-1), where A001383(n+1) = E(n) is
 *            the Euler transform of b(k) = A000041(k-1): E(0) = 1, n*E(n) =
 *            Sum_{k=1..n} c(k)*E(n-k), c(k) = Sum_{d|k} d*A000041(d-1)
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 43, u64 n <= 95, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A235_CAP 40001

static seq_t A235_P[A235_CAP], A235_C[A235_CAP], A235_E[A235_CAP];
static idx_t a235_pc = 0;   /* P, C valid for indices < a235_pc */
static idx_t a235_ec = 0;   /* E valid for indices < a235_ec */
static int a235_fixed = -1; /* 1 if seq_t is a fixed-width type */
static int a235_exact = 1;  /* fixed width: values still fit, use exact split */

static void a235_build(idx_t M)
{
    idx_t j, i, d, k;
    A235_P[0] = 1;
    for (j = 1; j <= M; j++) {
        seq_t pos = 0, neg = 0;
        for (i = 1;; i++) {
            idx_t g1 = i * (3 * i - 1) / 2;
            if (g1 > j) break;
            if (i & 1) pos += A235_P[j - g1]; else neg += A235_P[j - g1];
            if (g1 + i <= j) {
                if (i & 1) pos += A235_P[j - g1 - i]; else neg += A235_P[j - g1 - i];
            }
        }
        A235_P[j] = pos - neg;
    }
    for (k = 0; k <= M; k++) A235_C[k] = 0;
    for (d = 1; d <= M; d++) {
        seq_t t = A235_P[d - 1] * d;
        for (k = d; k <= M; k += d) A235_C[k] += t;
    }
    a235_pc = M + 1;
}

seq_t a(idx_t n)
{
    idx_t N, m, k;
    if (n < 1) return 0;
    N = n - 1;
    if (N >= A235_CAP) return 0;

    if (a235_fixed < 0) {
        seq_t t = 1;
        int i;
        for (i = 0; i < 200; i++) t = t + t;
        a235_fixed = (t == 0) ? 1 : 0;
    }

    if (N >= a235_pc) {
        idx_t M = 2 * a235_pc;
        if (M < N) M = N;
        if (M < 64) M = 64;
        if (M > A235_CAP - 1) M = A235_CAP - 1;
        a235_build(M);
    }

    if (a235_ec == 0) {
        A235_E[0] = 1;
        a235_ec = 1;
    }

    for (m = a235_ec; m <= N; m++) {
        if (!a235_fixed) {
            seq_t s = 0;
            for (k = 1; k <= m; k++) s += A235_C[k] * A235_E[m - k];
            A235_E[m] = s / m;
        } else if (!a235_exact) {
            /* values no longer fit: anything modular is fine, avoid int promotion */
            seq_t s = 0;
            for (k = 1; k <= m; k++) s += A235_C[k] * (A235_E[m - k] * (idx_t)1);
            A235_E[m] = s / m;
        } else {
            /* exact: split each product c*e into quotient/remainder modulo m */
            seq_t Q = 0;
            uint64_t R = 0;
            int ovf = 0;
            for (k = 1; k <= m; k++) {
                seq_t c = A235_C[k], e = A235_E[m - k];
                seq_t q1 = c / m;
                uint64_t r1 = (uint64_t)(long long)(c % m);
                seq_t q2 = e / m;
                uint64_t r2 = (uint64_t)(long long)(e % m);
                seq_t t = q1 * (q2 * m) + q1 * r2 + r1 * q2;
                seq_t old = Q;
                Q += t;
                if (Q < old) ovf = 1;
                R += r1 * r2;
            }
            Q += R / m;
            A235_E[m] = Q;
            if (ovf || (m >= 2 && A235_E[m] < A235_E[m - 1])) a235_exact = 0;
        }
    }
    if (N + 1 > a235_ec) a235_ec = N + 1;

    return A235_E[N] - A235_P[N];
}
