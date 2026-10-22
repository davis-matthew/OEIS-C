/*
 * A007120  Expansion of e.g.f. (1+x)^(1-x).
 * https://oeis.org/A007120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n+1) = Sum_{k=0..n} binomial(n,k)*b(k)*a(n-k), a(0) = 1, where
 *            b(0) = 1, b(k) = (-1)^k*(k-1)!*(2k+1) for k >= 1 (b(k) = k!
 *            [x^k] d/dx log((1+x)^(1-x)) = -log(1+x) + (1-x)/(1+x))
 * Range:     i8 n <= 6, i16 n <= 11, i32 n <= 16, i64 n <= 23, i128 n <= 37,
 *            big any n
 * Helpers:   from_i128, grow_zero, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_zero.h"
#include "../utils/from_i128.h"
#include "../utils/seq_is_fixed_width.h"

/* ---------- arbitrary precision / generic seq_t version ---------- */
static seq_t *SA = 0, *SB = 0, *SR = 0;
static idx_t scnt = 0, scap = 0;
static seq_t sfact;

static void s_ensure(idx_t n) {
    idx_t m, k;
    if (n < scnt) return;
    if (n + 1 > scap) {
        idx_t nc = scap * 2 > n + 1 ? scap * 2 : n + 1;
        SA = grow_zero(SA, scap, nc);
        SB = grow_zero(SB, scap, nc);
        SR = grow_zero(SR, scap, nc);
        scap = nc;
    }
    if (scnt == 0) {
        SA[0] = 1; SB[0] = 1; SR[0] = 1;
        sfact = 1;
        scnt = 1;
    }
    for (m = scnt; m <= n; m++) {
        seq_t s = 0, t;
        /* SR holds row m-1 of Pascal's triangle */
        for (k = 0; k < m; k++)
            s += SR[k] * SB[k] * SA[m - 1 - k];
        SA[m] = s;
        if (m >= 2) sfact *= (seq_t)(m - 1);
        t = sfact * (seq_t)(2 * m + 1);
        if (m & 1) t = 0 - t;
        SB[m] = t;
        SR[m] = 0;
        for (k = m; k >= 1; k--) SR[k] += SR[k - 1];
    }
    scnt = n + 1;
}

/* ---------- fixed-width version: exact modulo 2^128 (wrapping) ---------- */
typedef unsigned __int128 u128;
static u128 *UA = 0, *UB = 0, *UR = 0;
static idx_t ucnt = 0, ucap = 0;
static u128 ufact;

static void u_ensure(idx_t n) {
    idx_t m, k;
    if (n < ucnt) return;
    if (n + 1 > ucap) {
        idx_t nc = ucap * 2 > n + 1 ? ucap * 2 : n + 1;
        UA = (u128 *)realloc(UA, nc * sizeof(u128));
        UB = (u128 *)realloc(UB, nc * sizeof(u128));
        UR = (u128 *)realloc(UR, nc * sizeof(u128));
        ucap = nc;
    }
    if (ucnt == 0) {
        UA[0] = 1; UB[0] = 1; UR[0] = 1;
        ufact = 1;
        ucnt = 1;
    }
    for (m = ucnt; m <= n; m++) {
        u128 s = 0, t;
        for (k = 0; k < m; k++)
            s += UR[k] * UB[k] * UA[m - 1 - k];
        UA[m] = s;
        if (m >= 2) ufact *= (u128)(m - 1);
        t = ufact * (u128)(2 * m + 1);
        if (m & 1) t = (u128)0 - t;
        UB[m] = t;
        UR[m] = 0;
        for (k = m; k >= 1; k--) UR[k] += UR[k - 1];
    }
    ucnt = n + 1;
}

seq_t a(idx_t n) {
    if (seq_is_fixed_width()) {
        u_ensure(n);
        return from_i128((__int128)UA[n]);
    }
    s_ensure(n);
    return SA[n];
}
