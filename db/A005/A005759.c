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
 * Helpers:   alloc_zero, div_A, div_A3, mul_A3, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/div_A.h"
#include "../utils/div_A3.h"
#include "../utils/mul_A3.h"

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
