/*
 * A005759  Coefficients of modular function g_6(tau).
 * https://oeis.org/A005759
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [q^(n-4)] ( R(q)*(5*T(q) + q*T'(q)) + 13*q*T(q) ) / 5,
 *            where T(q) = Product_{k>=1} ((1-q^(11k))/(1-q^k))^12 (so q^5*T =
 *            (eta(11tau)/eta(tau))^12) and R(q) = Product_{k>=1}
 *            1/((1-q^k)*(1-q^(11k)))^2. This is g_6 = t*(5E + 13f)/(5f) on
 *            Gamma0(11), where t is the eta quotient, f = eta(tau)^2...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 25 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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

static bool seq_is_fixed_width(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 256; i++) {
        t *= 2;
        if (t == 0)
            return true;
    }
    return false;
}

/* s := s / A(q^step), pentagonal number theorem */
static void div_A(seq_t *s, idx_t L, idx_t step)
{
    idx_t m, j, g1, g2;
    for (m = 0; m < L; m++) {
        seq_t plus = s[m], minus = 0;
        for (j = 1; (g1 = step * (j * (3 * j - 1) / 2)) <= m; j++) {
            g2 = step * (j * (3 * j + 1) / 2);
            if (j & 1) {
                plus += s[m - g1];
                if (g2 <= m) plus += s[m - g2];
            } else {
                minus += s[m - g1];
                if (g2 <= m) minus += s[m - g2];
            }
        }
        s[m] = plus - minus;
    }
}

/* s := s / A(q^step)^3, using A^3 = Sum_{k>=0} (-1)^k (2k+1) q^(k(k+1)/2) */
static void div_A3(seq_t *s, idx_t L, idx_t step)
{
    idx_t m, k, t;
    for (m = 0; m < L; m++) {
        seq_t plus = s[m], minus = 0;
        for (k = 1; (t = step * (k * (k + 1) / 2)) <= m; k++) {
            seq_t term = s[m - t] * (seq_t)(2 * k + 1);
            if (k & 1) plus += term; else minus += term;
        }
        s[m] = plus - minus;
    }
}

/* s := s * A(q^step)^3 */
static void mul_A3(seq_t *s, idx_t L, idx_t step)
{
    idx_t m, k, t;
    for (m = L; m-- > 0;) {
        seq_t plus = s[m], minus = 0;
        for (k = 1; (t = step * (k * (k + 1) / 2)) <= m; k++) {
            seq_t term = s[m - t] * (seq_t)(2 * k + 1);
            if (k & 1) minus += term; else plus += term;
        }
        s[m] = plus - minus;
    }
}

static seq_t *F = NULL;
static idx_t cap = 0;

static void build(idx_t L)
{
    seq_t *T = alloc_zero(L);
    seq_t *Z = alloc_zero(L);
    seq_t *NF = alloc_zero(L);
    idx_t m;
    int i;
    bool fixed = seq_is_fixed_width();
    seq_t inv5 = 5;

    T[0] = 1;
    for (i = 0; i < 4; i++) div_A3(T, L, 1);
    for (i = 0; i < 4; i++) mul_A3(T, L, 11);

    for (m = 0; m < L; m++) Z[m] = T[m] * (seq_t)(m + 5);
    div_A(Z, L, 1);
    div_A(Z, L, 1);
    div_A(Z, L, 11);
    div_A(Z, L, 11);

    if (fixed)
        for (i = 0; i < 8; i++) inv5 = (seq_t)(inv5 * (seq_t)(2 - (seq_t)(5 * inv5)));

    for (m = 0; m < L; m++) {
        seq_t v = Z[m];
        if (m >= 1) v += T[m - 1] * 13;
        if (fixed) NF[m] = (seq_t)(v * inv5);
        else NF[m] = v / 5;
    }

    release(T, L);
    release(Z, L);
    if (F) release(F, cap);
    F = NF;
    cap = L;
}

seq_t a(idx_t n)
{
    idx_t m = n - 4;
    if (m >= cap) {
        idx_t L = m + 1;
        if (L < 2 * cap) L = 2 * cap;
        if (L < 64) L = 64;
        build(L);
    }
    return F[m];
}
