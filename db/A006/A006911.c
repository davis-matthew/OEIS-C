/*
 * A006911  Theta series of laminated lattice LAMBDA_11^{max}.
 * https://oeis.org/A006911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: theta_3(q^2)^11 + 26*theta_3(q^2)^7*theta_2(q^2)^4 +
 *            24*theta_3(q^2)^5*theta_2(q^2)^6 +
 *            13*theta_3(q^2)^3*theta_2(q^2)^8, where theta_2(q^2) =
 *            q^(1/2)*T(q) with T(q) = Sum_{k in Z} q^(2k(k+1)), so a(n) =
 *            [q^n] t3^11 + 26*[q^(n-2)] t3^7*T^4 + 24*[q^(n-3)] t3^5*T^6 +
 *            13*[q^(n-4)] t3^3*T^8, t3...
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 73, u64 n <= 8941,
 *            u128 n <= 8941, big any n
 * Helpers:   alloc_zero, release, sparse_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/sparse_mul.h"

static seq_t *P3[12];
static seq_t *PT[9];
static idx_t cap = 0;

static void ensure(idx_t n) {
    if (n + 1 <= cap) return;
    for (int k = 0; k < 12; k++)
        if (P3[k]) release(P3[k], cap);
    for (int k = 0; k < 9; k++)
        if (PT[k]) release(PT[k], cap);
    idx_t len = n + 1 + n / 4 + 16;
    for (int k = 0; k < 12; k++) P3[k] = alloc_zero(len);
    for (int k = 0; k < 9; k++) PT[k] = alloc_zero(len);
    P3[0][0] = 1;
    PT[0][0] = 1;
    for (int k = 1; k < 12; k++) sparse_mul(P3[k], P3[k - 1], len, 0);
    for (int k = 1; k < 9; k++) sparse_mul(PT[k], PT[k - 1], len, 1);
    cap = len;
}

static seq_t conv_at(const seq_t *x, const seq_t *y, idx_t m) {
    seq_t s = 0;
    for (idx_t i = 0; i <= m; i++)
        s += x[i] * y[m - i];
    return s;
}

seq_t a(idx_t n) {
    ensure(n);
    seq_t r = P3[11][n];
    if (n >= 2) r += 26 * conv_at(P3[7], PT[4], n - 2);
    if (n >= 3) r += 24 * conv_at(P3[5], PT[6], n - 3);
    if (n >= 4) r += 13 * conv_at(P3[3], PT[8], n - 4);
    return r;
}
