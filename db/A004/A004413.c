/*
 * A004413  Expansion of ( Sum_{n = -infinity..infinity} x^(n^2) )^(-12).
 * https://oeis.org/A004413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   with G(x) = theta_4(x)^(-12) = Sum G(n) x^n, a(n) = (-1)^n *
 *            G(n), where n*G(n) = Sum_{j>=1, j^2<=n} 2*(-1)^(j+1)*(11*j^2 +
 *            n)*G(n-j^2), G(0) = 1 (from theta_4*G' = -12*theta_4'*G,
 *            theta_4(x) = Sum_{k} (-1)^k x^(k^2)). For fixed-width builds:
 *            G(n) = -Sum_{k=1..n} T(k)*G(n-k), T = theta_4^12, comp...
 * Range:     i8 n <= 1, i16 n <= 4, i32 n <= 11, i64 n <= 33, i128 n <= 104,
 *            big any n
 * Helpers:   alloc_zero, from_i128, grow_zero, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/from_i128.h"

typedef unsigned __int128 a_u128;

/* fixed-width path: everything mod 2^128, only ring operations */
#define A_FW_MAX 300
static a_u128 a_fw_G[A_FW_MAX + 1];
static bool a_fw_done = false;

static void a_fw_init(void) {
    static a_u128 T[A_FW_MAX + 1], U[A_FW_MAX + 1];
    idx_t i, k, j;
    int r;
    for (i = 0; i <= A_FW_MAX; i++) T[i] = 0;
    T[0] = 1;
    for (r = 0; r < 12; r++) {
        for (i = 0; i <= A_FW_MAX; i++) U[i] = T[i];
        for (j = 1; j * j <= A_FW_MAX; j++) {
            a_u128 c = (j & 1) ? (a_u128)0 - (a_u128)2 : (a_u128)2;
            for (i = j * j; i <= A_FW_MAX; i++) U[i] += c * T[i - j * j];
        }
        for (i = 0; i <= A_FW_MAX; i++) T[i] = U[i];
    }
    a_fw_G[0] = 1;
    for (i = 1; i <= A_FW_MAX; i++) {
        a_u128 s = 0;
        for (k = 1; k <= i; k++) s += T[k] * a_fw_G[i - k];
        a_fw_G[i] = (a_u128)0 - s;
    }
    a_fw_done = true;
}

/* arbitrary-precision path */
static seq_t *a_G = NULL;
static idx_t a_cap = 0;
static idx_t a_cnt = 0;

seq_t a(idx_t n) {
    if (seq_is_fixed_width()) {
        a_u128 v;
        if (n > A_FW_MAX) return 0; /* far beyond any 128-bit value */
        if (!a_fw_done) a_fw_init();
        v = a_fw_G[n];
        if (n & 1) v = (a_u128)0 - v;
        return from_i128((__int128)v);
    }

    if (a_G == NULL) {
        a_cap = 64;
        a_G = alloc_zero(a_cap);
        a_G[0] = 1;
        a_cnt = 1;
    }
    if (n >= a_cap) {
        idx_t nc = a_cap;
        while (nc <= n) nc *= 2;
        a_G = grow_zero(a_G, a_cap, nc);
        a_cap = nc;
    }
    while (a_cnt <= n) {
        idx_t m = a_cnt, j;
        seq_t s = 0;
        for (j = 1; j * j <= m; j++) {
            idx_t jj = j * j;
            seq_t t = a_G[m - jj] * (seq_t)(2 * (11 * jj + m));
            if (j & 1) s += t; else s -= t;
        }
        a_G[m] = s / (seq_t)m;
        a_cnt++;
    }
    if (n & 1) return -a_G[n];
    return a_G[n];
}
