/*
 * A003494  High temperature series for susceptibility for spherical model on
 *          b.c.c. lattice.
 * https://oeis.org/A003494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   chi = Y(x^2)/(1 - 8*x*Y(x^2)), where z = x*Y(x^2) is the series
 *            reversion of x = z*P(z^2), P(w) = Sum_{m>=0} binomial(2m,m)^3 *
 *            w^m (bcc lattice Green function); equivalently 1/Y = P(u*Y^2)
 *            with u = x^2, Y(0) = 1, and a(n) = [x^n] chi
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   A_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/A_grow.h"

static int g_big = -1;
static int is_big(void)
{
    if (g_big < 0) {
        seq_t x = 1;
        int i;
        for (i = 0; i < 300; i++) x = x * 2;
        g_big = (x != 0);
    }
    return g_big;
}

/* signed values: magnitude + sign in big mode, plain wraparound in fixed mode */
static void s_add(seq_t am, int as, seq_t bm, int bs, seq_t *rm, int *rs)
{
    seq_t m;
    int s;
    if (!is_big()) { *rm = am + bm; *rs = 0; return; }
    if (as == bs) { m = am + bm; s = as; }
    else if (am >= bm) { m = am - bm; s = as; }
    else { m = bm - am; s = bs; }
    if (m == 0) s = 0;
    *rm = m; *rs = s;
}

static void s_mul(seq_t am, int as, seq_t bm, int bs, seq_t *rm, int *rs)
{
    seq_t m = am * bm;
    int s = is_big() ? (as ^ bs) : 0;
    if (m == 0) s = 0;
    *rm = m; *rs = s;
}

static void s_neg(seq_t *m, int *s)
{
    if (!is_big()) { *m = (seq_t)0 - *m; *s = 0; }
    else if (*m != 0) *s = !*s;
}

static seq_t *Ym, *Y2m, *Wm, *Pm, *Am, *Bm, *Cm, *Rm;
static int *Ys, *Y2s, *Ws, *Ps, *As, *Rs;
static idx_t cap = 0, done = 0, rcap = 0;

static void ensure(idx_t D)
{
    idx_t k, m, j;
    if (D + 1 > cap) {
        idx_t nc = 2 * (D + 1);
        if (nc < 16) nc = 16;
        Ym = A_grow(Ym, cap, nc);
        Y2m = A_grow(Y2m, cap, nc);
        Wm = A_grow(Wm, cap, nc);
        Am = A_grow(Am, cap, nc);
        Bm = A_grow(Bm, cap, nc);
        Cm = A_grow(Cm, cap, nc);
        Pm = A_grow(Pm, cap * cap, nc * nc);
        Ys = (int *)realloc(Ys, (size_t)nc * sizeof(int));
        Y2s = (int *)realloc(Y2s, (size_t)nc * sizeof(int));
        Ws = (int *)realloc(Ws, (size_t)nc * sizeof(int));
        As = (int *)realloc(As, (size_t)nc * sizeof(int));
        Ps = (int *)realloc(Ps, (size_t)(nc * nc) * sizeof(int));
        cap = nc;
        done = 0;
    }
    for (k = done; k <= D; k++) {
        seq_t tm, sm;
        int ts, ss;
        if (k == 0) {
            Ym[0] = 1; Ys[0] = 0;
            Y2m[0] = 1; Y2s[0] = 0;
            Wm[0] = 0; Ws[0] = 0;
            Am[0] = 1; As[0] = 0;
            Bm[0] = 1; Cm[0] = 1;
            continue;
        }
        Bm[k] = Bm[k - 1] * (seq_t)(4 * k - 2) / (seq_t)k;
        Cm[k] = Bm[k] * Bm[k] * Bm[k];
        Wm[k] = Y2m[k - 1]; Ws[k] = Y2s[k - 1];
        for (m = 1; m <= k; m++) {
            if (m == 1) {
                Pm[cap + k] = Wm[k]; Ps[cap + k] = Ws[k];
                continue;
            }
            sm = 0; ss = 0;
            for (j = 1; j <= k - m + 1; j++) {
                s_mul(Wm[j], Ws[j], Pm[(m - 1) * cap + (k - j)], Ps[(m - 1) * cap + (k - j)], &tm, &ts);
                s_add(sm, ss, tm, ts, &sm, &ss);
            }
            Pm[m * cap + k] = sm; Ps[m * cap + k] = ss;
        }
        sm = 0; ss = 0;
        for (m = 1; m <= k; m++) {
            s_mul(Cm[m], 0, Pm[m * cap + k], Ps[m * cap + k], &tm, &ts);
            s_add(sm, ss, tm, ts, &sm, &ss);
        }
        Am[k] = sm; As[k] = ss;
        sm = 0; ss = 0;
        for (j = 1; j <= k; j++) {
            s_mul(Am[j], As[j], Ym[k - j], Ys[k - j], &tm, &ts);
            s_add(sm, ss, tm, ts, &sm, &ss);
        }
        s_neg(&sm, &ss);
        Ym[k] = sm; Ys[k] = ss;
        sm = 0; ss = 0;
        for (j = 0; j <= k; j++) {
            s_mul(Ym[j], Ys[j], Ym[k - j], Ys[k - j], &tm, &ts);
            s_add(sm, ss, tm, ts, &sm, &ss);
        }
        Y2m[k] = sm; Y2s[k] = ss;
    }
    if (D + 1 > done) done = D + 1;
}

seq_t a(idx_t n)
{
    idx_t i, e;
    seq_t sm, tm;
    int ss, ts;
    ensure(n / 2);
    if (n + 1 > rcap) {
        idx_t nc = 2 * (n + 1);
        Rm = A_grow(Rm, rcap, nc);
        Rs = (int *)realloc(Rs, (size_t)nc * sizeof(int));
        rcap = nc;
    }
    /* r = 1/(1 - 8 x Y(x^2)) */
    Rm[0] = 1; Rs[0] = 0;
    for (i = 1; i <= n; i++) {
        sm = 0; ss = 0;
        for (e = 0; e <= i - 1; e += 2) {
            s_mul(Ym[e / 2], Ys[e / 2], Rm[i - 1 - e], Rs[i - 1 - e], &tm, &ts);
            s_mul(tm, ts, (seq_t)8, 0, &tm, &ts);
            s_add(sm, ss, tm, ts, &sm, &ss);
        }
        Rm[i] = sm; Rs[i] = ss;
    }
    sm = 0; ss = 0;
    for (e = 0; e <= n; e += 2) {
        s_mul(Ym[e / 2], Ys[e / 2], Rm[n - e], Rs[n - e], &tm, &ts);
        s_add(sm, ss, tm, ts, &sm, &ss);
    }
    return sm;
}
