/*
 * A004671  Theta series of extremal even unimodular lattice in dimension 40.
 * https://oeis.org/A004671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A282015(n) - 1200*A037945(n), i.e. theta = E4^2*(E4^3 -
 *            1200*Delta) = E8 * G with 691*G = 691*E12 - 397200*Delta, so
 *            a(n) = Sum_{i=0..n} e8(i)*g(n-i), e8(0)=1, e8(i)=480*sigma_7(i),
 *            691*g(m) = 691*[m=0] + 65520*sigma_11(m) - 397200*tau(m); tau
 *            via Delta/q = (Sum (-1)^k (2k+1) q^(k(k+1)/2))...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 11, u128 n <= 122,
 *            big any n
 * Helpers:   grow_zero, seq_is_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_is_big.h"
#include "../utils/grow_zero.h"
#include "../utils/u128_to_seq.h"

typedef unsigned __int128 u128;

/* ---------- arbitrary-precision path (sign-magnitude) ---------- */
static seq_t *b_s7, *b_s11, *b_F, *b_G;
static unsigned char *b_Fs, *b_Gs;
static idx_t b_cap = 0;

static void big_build(idx_t N)
{
    idx_t nc = N + 1, m, d;
    b_s7 = grow_zero(b_s7, b_cap, nc);
    b_s11 = grow_zero(b_s11, b_cap, nc);
    b_F = grow_zero(b_F, b_cap, nc);
    b_G = grow_zero(b_G, b_cap, nc);
    b_Fs = (unsigned char *)realloc(b_Fs, (size_t)nc);
    b_Gs = (unsigned char *)realloc(b_Gs, (size_t)nc);
    for (m = 0; m < nc; m++) { b_s7[m] = 0; b_s11[m] = 0; }
    for (d = 1; d <= N; d++) {
        seq_t x = (seq_t)d, x2 = x * x, x4 = x2 * x2, x7 = x4 * x2 * x, x11 = x7 * x4;
        for (m = d; m <= N; m += d) { b_s7[m] += x7; b_s11[m] += x11; }
    }
    /* F = P^8, P = prod (1-q^k)^3 (sparse, Jacobi) */
    b_F[0] = 1; b_Fs[0] = 0;
    for (m = 1; m <= N; m++) {
        seq_t pos = 0, neg = 0;
        idx_t k;
        for (k = 1; k * (k + 1) / 2 <= m; k++) {
            idx_t t = k * (k + 1) / 2;
            long long pj = (long long)(2 * k + 1);
            long long c;
            int sg;
            if (k & 1) pj = -pj;
            c = (9LL * (long long)t - (long long)m) * pj;
            if (c == 0) continue;
            sg = (c < 0) ^ b_Fs[m - t];
            {
                seq_t term = b_F[m - t] * (seq_t)(idx_t)(c < 0 ? -c : c);
                if (sg) neg += term; else pos += term;
            }
        }
        if (pos >= neg) { b_F[m] = (pos - neg) / (seq_t)m; b_Fs[m] = 0; }
        else { b_F[m] = (neg - pos) / (seq_t)m; b_Fs[m] = 1; }
    }
    for (m = 0; m <= N; m++) {
        seq_t pos = b_s11[m] * 65520, neg = 0;
        if (m == 0) pos += 691;
        else {
            seq_t term = b_F[m - 1] * 397200;
            if (b_Fs[m - 1]) pos += term; else neg += term;
        }
        if (pos >= neg) { b_G[m] = (pos - neg) / 691; b_Gs[m] = 0; }
        else { b_G[m] = (neg - pos) / 691; b_Gs[m] = 1; }
    }
    b_cap = nc;
}

static seq_t big_a(idx_t n)
{
    seq_t pos = 0, neg = 0;
    idx_t i;
    if (n + 1 > b_cap) {
        idx_t N = 2 * b_cap;
        if (N < n) N = n;
        big_build(N);
    }
    for (i = 0; i <= n; i++) {
        seq_t term;
        if (i == 0) term = b_G[n];
        else term = b_s7[i] * 480 * b_G[n - i];
        if (b_Gs[n - i]) neg += term; else pos += term;
    }
    return pos - neg;
}

/* ---------- fixed-width path (exact modulo 2^128) ---------- */
static u128 *f_e4, *f_e8, *f_b, *f_y, *f_th;
static idx_t f_len = 0, f_cap = 0;

static u128 *f_grow(u128 *old, idx_t oldlen, idx_t newlen)
{
    idx_t i;
    u128 *p = (u128 *)realloc(old, (size_t)newlen * sizeof(u128));
    for (i = oldlen; i < newlen; i++) p[i] = 0;
    return p;
}

static void fix_extend(idx_t n)
{
    idx_t m, i;
    if (n < f_len) return;
    if (n >= f_cap) {
        idx_t nc = f_cap * 2;
        if (nc < n + 1) nc = n + 1;
        f_e4 = f_grow(f_e4, f_cap, nc);
        f_e8 = f_grow(f_e8, f_cap, nc);
        f_b = f_grow(f_b, f_cap, nc);
        f_y = f_grow(f_y, f_cap, nc);
        f_th = f_grow(f_th, f_cap, nc);
        f_cap = nc;
    }
    for (m = f_len; m <= n; m++) {
        u128 cm = 0, b2m = 0, pos, neg, diff, t = 0;
        if (m == 0) {
            f_e4[0] = 1; f_e8[0] = 1; f_b[0] = 0;
        } else {
            u128 s3 = 0, s5 = 0, s7 = 0;
            idx_t d;
            for (d = 1; d * d <= m; d++) {
                if (m % d == 0) {
                    idx_t e = m / d;
                    u128 x = d, x2 = x * x, x3 = x2 * x, x5 = x3 * x2;
                    s3 += x3; s5 += x5; s7 += x5 * x2;
                    if (e != d) {
                        u128 y = e, y2 = y * y, y3 = y2 * y, y5 = y3 * y2;
                        s3 += y3; s5 += y5; s7 += y5 * y2;
                    }
                }
            }
            f_e4[m] = s3 * 240;
            f_e8[m] = s7 * 480;
            f_b[m] = s5;
        }
        for (i = 0; i <= m; i++) cm += f_e4[i] * f_e8[m - i];
        for (i = 1; i < m; i++) b2m += f_b[i] * f_b[m - i];
        pos = cm * 11 + b2m * 6350400;
        if (m == 0) pos += 25;
        neg = f_b[m] * 25200;
        diff = pos - neg;
        if (diff >> 127) f_y[m] = (u128)0 - ((((u128)0) - diff) / 36);
        else f_y[m] = diff / 36;
        for (i = 0; i <= m; i++) t += f_e8[i] * f_y[m - i];
        f_th[m] = t;
    }
    f_len = n + 1;
}

seq_t a(idx_t n)
{
    if (seq_is_big()) return big_a(n);
    fix_extend(n);
    return u128_to_seq(f_th[n]);
}
