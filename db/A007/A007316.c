/*
 * A007316  Reversion of g.f. for Euler numbers A000111(n-1).
 * https://oeis.org/A007316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   with F(x) = Sum_{i>=0} A000111(i) x^i and H(x) = 1/F(x) = Sum
 *            h_i x^i, a(n) = (1/n) [x^(n-1)] H(x)^n (Lagrange inversion of
 *            x*F(x)); equivalently g = Sum a(n) x^n satisfies g = x*H(g),
 *            a(1) = 1. A000111 is computed from the Entringer triangle E(m,k)
 *            = E(m,k-1) + E(m-1,m-k), E(0,0) = 1
 * Range:     i8 n <= 9, i16 n <= 11, i32 n <= 16, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/fixed_width.h"

#define MAXN 16400
#define FN 160

static seq_t Eu[MAXN], Hc[MAXN], R0[MAXN], R1[MAXN], Cc[MAXN];
static idx_t have = 0;

static seq_t P[FN][FN];
static seq_t G[FN];
static idx_t gcount = 0;

static void build(idx_t N) {
    idx_t m, k, i;
    seq_t *p, *q, *t;
    if (N <= have) return;
    if (N < 2 * have) N = 2 * have;
    if (N > MAXN) N = MAXN;
    p = R0; q = R1;
    p[0] = 1;
    Eu[0] = 1;
    for (m = 1; m < N; m++) {
        q[0] = 0;
        for (k = 1; k <= m; k++) q[k] = q[k - 1] + p[m - k];
        Eu[m] = q[m];
        t = p; p = q; q = t;
    }
    Hc[0] = 1;
    for (k = 1; k < N; k++) {
        seq_t s = 0;
        for (i = 1; i <= k; i++) s -= Eu[i] * Hc[k - i];
        Hc[k] = s;
    }
    have = N;
}

static seq_t fixed_path(idx_t n) {
    idx_t m, k, i;
    if (n >= FN) return 0;
    build(n + 1);
    if (gcount == 0) {
        G[1] = 1;
        P[1][1] = 1;
        gcount = 1;
    }
    for (m = gcount + 1; m <= n; m++) {
        idx_t j = m - 1;
        seq_t s = 0;
        for (k = 2; k <= j; k++) {
            seq_t v = 0;
            for (i = 1; i + k - 1 <= j; i++) v += G[i] * P[k - 1][j - i];
            P[k][j] = v;
        }
        for (k = 1; k <= j; k++) s += Hc[k] * P[k][j];
        G[m] = s;
        P[1][m] = s;
        gcount = m;
    }
    return G[n];
}

static seq_t big_path(idx_t n) {
    idx_t k, i;
    if (n >= MAXN) return 0;
    build(n + 1);
    Cc[0] = 1;
    for (k = 1; k < n; k++) {
        seq_t s = 0;
        for (i = 1; i <= k; i++) {
            seq_t coef = (seq_t)(n + 1) * (seq_t)i - (seq_t)k;
            s += coef * Hc[i] * Cc[k - i];
        }
        Cc[k] = s / (seq_t)k;
    }
    return Cc[n - 1] / (seq_t)n;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (fixed_width()) return fixed_path(n);
    return big_path(n);
}
