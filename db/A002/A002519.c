/*
 * A002519  Theta series of 28-dimensional unimodular lattice with no roots
 *          and a parity vector of norm 4.
 * https://oeis.org/A002519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. = t3^28 - 56*t3^20*D8 + 280*t3^12*D8^2 - 512*t3^4*D8^3, D8
 *            = (theta2*theta4)^4/16. Using theta4^4 = theta3^4 - theta2^4,
 *            with U = theta3^4 and T = theta2^4/16 = q*P^4, P = Sum_{m>=0}
 *            q^(m(m+1)), this becomes U^7 - 56*T*U^6 + 1176*T^2*U^5 -
 *            9472*T^3*U^4 + 96256*T^4*U^3 - 393216*T^5*U^2 + 2097...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 9, u64 n <= 50, u128 n <= 1520,
 *            big any n
 * Helpers:   a2519_mul_P, a2519_mul_t3, alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/a2519_mul_P.h"
#include "../utils/a2519_mul_t3.h"

static seq_t *a2519_U[8];   /* a2519_U[k] = theta3^(4k), k = 1..7 */
static seq_t *a2519_P[7];   /* a2519_P[j] = P^(4j),      j = 1..6 */
static idx_t a2519_cap = 0;

static void a2519_build(idx_t N) {
    if (a2519_cap > 0) {
        for (int k = 1; k <= 7; k++) release(a2519_U[k], a2519_cap);
        for (int j = 1; j <= 6; j++) release(a2519_P[j], a2519_cap);
    }
    for (int k = 1; k <= 7; k++) {
        seq_t *v = alloc_zero(N);
        if (k == 1) v[0] = 1;
        else for (idx_t i = 0; i < N; i++) v[i] = a2519_U[k - 1][i];
        for (int r = 0; r < 4; r++) a2519_mul_t3(v, N);
        a2519_U[k] = v;
    }
    for (int j = 1; j <= 6; j++) {
        seq_t *v = alloc_zero(N);
        if (j == 1) v[0] = 1;
        else for (idx_t i = 0; i < N; i++) v[i] = a2519_P[j - 1][i];
        for (int r = 0; r < 4; r++) a2519_mul_P(v, N);
        a2519_P[j] = v;
    }
    a2519_cap = N;
}

/* coefficient of q^n in T^j * U^(7-j), with T^j = q^j * P^(4j) */
static seq_t a2519_coef(idx_t n, int j) {
    seq_t s = 0;
    const seq_t *pp = a2519_P[j];
    const seq_t *uu = a2519_U[7 - j];
    for (idx_t i = (idx_t)j; i <= n; i++)
        s += (pp[i - (idx_t)j] * 1u) * uu[n - i];
    return s;
}

seq_t a(idx_t n) {
    if (n + 1 > a2519_cap) {
        idx_t N = 2 * a2519_cap;
        if (N < n + 1) N = n + 1;
        if (N < 64) N = 64;
        a2519_build(N);
    }
    seq_t pos = a2519_U[7][n];
    pos += a2519_coef(n, 2) * 1176u;
    pos += a2519_coef(n, 4) * 96256u;
    pos += a2519_coef(n, 6) * 2097152u;
    seq_t neg = a2519_coef(n, 1) * 56u;
    neg += a2519_coef(n, 3) * 9472u;
    neg += a2519_coef(n, 5) * 393216u;
    return pos - neg;
}
