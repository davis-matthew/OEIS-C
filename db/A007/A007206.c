/*
 * A007206  Magnetization for honeycomb lattice.
 * https://oeis.org/A007206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   G.f. f(z) = (P(z)/D(z))^(1/8), where D = (1-z)^3*(1-z^2)^3 and P
 *            = D - 16*z^3 - 16*z^6. Differentiating gives 8*P*D*f' = (P'*D -
 *            P*D')*f. With Q = P*D (degree 18, Q_0 = 1) and R = P'*D - P*D',
 *            this is the recurrence a(n) = ( Sum_{j=0..17} R_j*a(n-1-j) -
 *            8*Sum_{j=1..18} Q_j*(n-j)*a(n-j) ) / (8*n),...
 * Range:     i8 n <= 6, i16 n <= 11, i32 n <= 20, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 29 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *grow_zero(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    if (p == NULL) return NULL;
    mem_set(p, 0, (size_t)newlen * sizeof(seq_t));
    if (old != NULL) {
        mem_copy(p, old, (size_t)oldlen * sizeof(seq_t));
        mem_free(old);
    }
    return p;
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

static seq_t from_i128(__int128 v)
{
    if (v >= -(__int128)4000000000000000000LL && v <= (__int128)4000000000000000000LL)
        return (seq_t)(long long)v;
    {
        __int128 q = v / ((__int128)1 << 32);
        long long rem = (long long)(v - q * ((__int128)1 << 32));
        return from_i128(q) * (seq_t)4294967296LL + (seq_t)rem;
    }
}

static long long Qc[19], Rc[19];
static bool polys_ready = false;

static void init_polys(void) {
    long long a3[4] = {1, -3, 3, -1};
    long long b3[7] = {1, 0, -3, 0, 3, 0, -1};
    long long D[10], P[10], dD[10], dP[10];
    int i, j;
    if (polys_ready) return;
    memset(D, 0, sizeof(D));
    for (i = 0; i < 4; i++)
        for (j = 0; j < 7; j++)
            D[i + j] += a3[i] * b3[j];
    for (i = 0; i < 10; i++) P[i] = D[i];
    P[3] -= 16;
    P[6] -= 16;
    memset(dD, 0, sizeof(dD));
    memset(dP, 0, sizeof(dP));
    for (i = 1; i < 10; i++) {
        dD[i - 1] = i * D[i];
        dP[i - 1] = i * P[i];
    }
    memset(Qc, 0, sizeof(Qc));
    memset(Rc, 0, sizeof(Rc));
    for (i = 0; i < 10; i++)
        for (j = 0; j < 10; j++) {
            Qc[i + j] += P[i] * D[j];
            Rc[i + j] += dP[i] * D[j] - P[i] * dD[j];
        }
    polys_ready = true;
}

/* fixed-width path: exact 128-bit intermediates */
#define I128_CAP 256
static __int128 ic[I128_CAP];
static long long ic_len = 0;
static bool ic_overflow = false;

static __int128 a_i128(idx_t n) {
    if (ic_len == 0) { ic[0] = 1; ic_len = 1; }
    while ((idx_t)ic_len <= n && !ic_overflow && ic_len < I128_CAP) {
        long long m = ic_len;
        __int128 s = 0;
        __int128 lim = (__int128)1 << 100;
        int j;
        for (j = 0; j <= 18; j++) {
            if (m - 1 - j >= 0) s += (__int128)Rc[j] * ic[m - 1 - j];
            if (j >= 1 && m - j >= 0) s -= (__int128)8 * Qc[j] * (m - j) * ic[m - j];
        }
        s /= (__int128)8 * m;
        ic[m] = s;
        ic_len++;
        if (s > lim || s < -lim) ic_overflow = true;
    }
    if ((idx_t)ic_len > n) return ic[n];
    return 0;
}

/* arbitrary-precision path */
static seq_t *bc = NULL;
static idx_t bc_len = 0, bc_cap = 0;

static seq_t a_big(idx_t n) {
    if (bc_cap == 0) {
        bc_cap = 64;
        bc = grow_zero(NULL, 0, bc_cap);
        bc[0] = 1;
        bc_len = 1;
    }
    if (n >= bc_cap) {
        idx_t nc = bc_cap;
        while (nc <= n) nc *= 2;
        bc = grow_zero(bc, bc_cap, nc);
        bc_cap = nc;
    }
    while (bc_len <= n) {
        idx_t m = bc_len;
        seq_t s = 0;
        int j;
        for (j = 0; j <= 18; j++) {
            if (m >= (idx_t)j + 1) s += (seq_t)Rc[j] * bc[m - 1 - j];
            if (j >= 1 && m >= (idx_t)j) s -= (seq_t)(8 * Qc[j]) * (seq_t)(m - j) * bc[m - j];
        }
        s /= (seq_t)8 * (seq_t)m;
        bc[m] = s;
        bc_len++;
    }
    return bc[n];
}

seq_t a(idx_t n) {
    init_polys();
    if (seq_is_fixed_width()) return from_i128(a_i128(n));
    return a_big(n);
}
