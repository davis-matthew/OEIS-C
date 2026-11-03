/*
 * A004536  Theta series of 15-dimensional unimodular lattice A15+.
 * https://oeis.org/A004536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   theta series of the odd unimodular lattice A15+ in dimension 15:
 *            Theta = theta_3^15 - 30*theta_3^7*Delta_8, where Delta_8 =
 *            q*prod_{m>=1}(1-q^(2m-1))^8*(1-q^(4m))^8 =
 *            q*psi(q^2)^4*theta_4(q)^4, psi(x) = Sum_{k>=0} x^(k(k+1)/2). So
 *            a(n) = [q^n]theta_3^15 -
 *            30*[q^(n-1)](theta_3^7*theta_4^4*psi(q^2)...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static void release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

/* f <- f * psi(q^2) = f * Sum_{k>=0} q^(k(k+1)) */
static void mul_psi2(seq_t *f, idx_t L) {
    idx_t i = L;
    while (i-- > 0) {
        seq_t s = f[i];
        idx_t k;
        for (k = 1; k * (k + 1) <= i; k++) s += f[i - k * (k + 1)];
        f[i] = s;
    }
}

/* f <- f * theta_3(q), in place, high index to low */
static void mul_theta3(seq_t *f, idx_t L) {
    idx_t i = L;
    while (i-- > 0) {
        seq_t s = f[i];
        idx_t k;
        for (k = 1; k * k <= i; k++) s += 2 * f[i - k * k];
        f[i] = s;
    }
}

/* (P - M) <- (P - M) * theta_4(q), theta_4 = 1 + 2*Sum (-1)^k q^(k^2) */
static void mul_theta4(seq_t *P, seq_t *M, idx_t L) {
    idx_t i = L;
    while (i-- > 0) {
        seq_t sp = P[i];
        seq_t sm = M[i];
        idx_t k;
        for (k = 1; k * k <= i; k++) {
            if (k & 1) {
                sp += 2 * M[i - k * k];
                sm += 2 * P[i - k * k];
            } else {
                sp += 2 * P[i - k * k];
                sm += 2 * M[i - k * k];
            }
        }
        P[i] = sp;
        M[i] = sm;
    }
}

static seq_t *S_arr = 0;   /* theta_3^15 */
static seq_t *P_arr = 0;   /* positive part of theta_3^7*theta_4^4*psi(q^2)^4 */
static seq_t *M_arr = 0;   /* negative part */
static idx_t cap = 0;

static void build(idx_t L) {
    idx_t i;
    int j;
    if (S_arr) {
        release(S_arr, cap);
        release(P_arr, cap);
        release(M_arr, cap);
    }
    S_arr = alloc_zero(L);
    P_arr = alloc_zero(L);
    M_arr = alloc_zero(L);
    cap = L;

    S_arr[0] = 1;
    for (j = 0; j < 7; j++) mul_theta3(S_arr, L);
    for (i = 0; i < L; i++) P_arr[i] = S_arr[i];
    for (j = 0; j < 8; j++) mul_theta3(S_arr, L);

    for (j = 0; j < 4; j++) mul_theta4(P_arr, M_arr, L);
    for (j = 0; j < 4; j++) {
        mul_psi2(P_arr, L);
        mul_psi2(M_arr, L);
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n + 1 > cap) {
        idx_t L = cap + cap / 4;
        if (L < n + 1) L = n + 1;
        if (L < 64) L = 64;
        build(L);
    }
    seq_t r = S_arr[n];
    r += 30 * M_arr[n - 1];
    r -= 30 * P_arr[n - 1];
    return r;
}
