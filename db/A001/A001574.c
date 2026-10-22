/*
 * A001574  Colored series-parallel networks.
 * https://oeis.org/A001574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = T_2(n) - 2*T_1(n), where T_c(n) counts series-parallel
 *            networks with n edges, each edge given one of c colors. So a(n)
 *            counts the networks that use both colors. T_c(1) = c, and T_c(n)
 *            = 2*A_c(n) for n >= 2, where A_c is defined by Prod_{k>=1}
 *            (1-x^k)^(-A_c(k)) = 1 + Sum_{n>=1} T_c(n) x^n,...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   a1574_g, a1574_is_fixed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a1574_g.h"
#include "../utils/a1574_is_fixed.h"

#define A1574_MAXN 10000
#define A1574_FIXN 200

/* Fixed-width path: multiply the product out factor by factor. Every intermediate
   value is no larger than the coefficient it feeds, so results that fit are exact. */
static seq_t a1574_fixed_T(idx_t n, unsigned c) {
    seq_t P[A1574_FIXN + 1];
    seq_t Cf[A1574_FIXN + 1];
    idx_t i, k, j, m;
    seq_t T = 0;
    for (i = 0; i <= n; i++) P[i] = 0;
    P[0] = 1;
    for (k = 1; k <= n; k++) {
        seq_t Ak = (k == 1) ? (seq_t)c : P[k];
        if (k == n) {
            T = (k == 1) ? Ak : (seq_t)2 * Ak;
            break;
        }
        Cf[0] = 1;
        for (j = 1; j * k <= n; j++) {
            seq_t x = Ak + (seq_t)(j - 1);
            idx_t r = (idx_t)(x % (seq_t)j);
            idx_t h = a1574_g(j, r);
            Cf[j] = (Cf[j - 1] / (seq_t)(j / h)) * (x / (seq_t)h);
        }
        for (m = n; m >= k; m--) {
            seq_t add = 0;
            for (j = 1; j * k <= m; j++) add += Cf[j] * P[m - j * k];
            P[m] += add;
        }
    }
    return T;
}

static seq_t a1574_A[2][A1574_MAXN + 1];
static seq_t a1574_B[2][A1574_MAXN + 1];
static seq_t a1574_C[2][A1574_MAXN + 1];
static idx_t a1574_done = 0;

static void a1574_big_extend(idx_t n) {
    idx_t m, k, d;
    int ci;
    if (a1574_done == 0) {
        a1574_C[0][0] = 1;
        a1574_C[1][0] = 1;
    }
    for (m = a1574_done + 1; m <= n; m++) {
        for (ci = 0; ci < 2; ci++) {
            seq_t cc = (seq_t)(ci + 1);
            if (m == 1) {
                a1574_A[ci][1] = cc;
                a1574_B[ci][1] = cc;
                a1574_C[ci][1] = cc;
            } else {
                seq_t s = 0, dsum = 0;
                for (k = 1; k < m; k++) s += a1574_B[ci][k] * a1574_C[ci][m - k];
                for (d = 1; d < m; d++)
                    if (m % d == 0) dsum += (seq_t)d * a1574_A[ci][d];
                s += dsum;
                a1574_A[ci][m] = s / (seq_t)m;
                a1574_B[ci][m] = dsum + (seq_t)m * a1574_A[ci][m];
                a1574_C[ci][m] = (seq_t)2 * a1574_A[ci][m];
            }
        }
    }
    if (n > a1574_done) a1574_done = n;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (a1574_is_fixed()) {
        if (n > A1574_FIXN) return 0;
        return a1574_fixed_T(n, 2) - (seq_t)2 * a1574_fixed_T(n, 1);
    }
    if (n > A1574_MAXN) return 0;
    if (n > a1574_done) a1574_big_extend(n);
    return a1574_C[1][n] - (seq_t)2 * a1574_C[0][n];
}
