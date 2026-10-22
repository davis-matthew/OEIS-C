/*
 * A004109  Number of connected trivalent (or cubic) labeled graphs with 2n
 *          nodes.
 * https://oeis.org/A004109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(0) = 1; a(n) = g(n) - Sum_{k=1..n-1}
 *            binomial(2n-1,2k-1)*a(k)*g(n-k), where g(n) = A002829(n) (all
 *            labeled cubic graphs on 2n nodes) = coefficient of
 *            x_1^3...x_{2n}^3 in exp(p1^2/2 - p2/2 - p2^2/4 + p3^2/6), i.e.
 *            6^(2n) g(n) = Sum_{d=0..n}
 *            binomial(2n,2d)*12^d*(2d-1)!!*Z(2n-2d), Z(M) = Sum_{A=0...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   grow_zero, sadd, seq_is_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/seq_is_big.h"
#include "../utils/grow_zero.h"
#include "../utils/u128_to_seq.h"
#include "../utils/sadd.h"

/* ---------- arbitrary precision path (sign-magnitude over seq_t) ---------- */

static seq_t *Tm = NULL, *Zm = NULL, *G = NULL, *Cc = NULL;
static int *Ts = NULL, *Zs = NULL;
static idx_t tlen = 0, zlen = 0, glen = 0;

static void big_extend(idx_t n)
{
    idx_t b, M, A, m, d, k, i;
    if (n + 1 <= glen) return;
    /* T(0..2n) */
    if (2 * n + 1 > tlen) {
        Tm = grow_zero(Tm, tlen, 2 * n + 1);
        Ts = (int *)realloc(Ts, (size_t)(2 * n + 1) * sizeof(int));
        for (b = tlen; b <= 2 * n; b++) {
            if (b == 0) { Tm[b] = 1; Ts[b] = 0; }
            else if (b == 1) { Tm[b] = 1; Ts[b] = 1; }
            else {
                seq_t r = 0; int rs = 0;
                sadd(&r, &rs, Tm[b - 1], !Ts[b - 1]);
                sadd(&r, &rs, Tm[b - 2] * (seq_t)(2 * (b - 1)), !Ts[b - 2]);
                Tm[b] = r; Ts[b] = rs;
            }
        }
        tlen = 2 * n + 1;
    }
    /* Z(M) for even M, stored at M/2 */
    if (n + 1 > zlen) {
        Zm = grow_zero(Zm, zlen, n + 1);
        Zs = (int *)realloc(Zs, (size_t)(n + 1) * sizeof(int));
        for (i = zlen; i <= n; i++) {
            seq_t zm = 0, c = 1, df = 1, p3 = 1;
            int zs = 0;
            idx_t j;
            M = 2 * i;
            for (j = 1; j + 1 <= M; j += 2) df *= (seq_t)j;
            for (j = 0; j < M; j++) p3 *= 3;
            for (A = 0; A <= M; A++) {
                idx_t B = M - A;
                seq_t term = c * df * Tm[B] * p3;
                sadd(&zm, &zs, term, Ts[B]);
                if (A < M) {
                    c = c * (seq_t)(M - A) / (seq_t)(A + 1);
                    df *= (seq_t)(M + 2 * A + 1);
                    p3 /= 3;
                }
            }
            Zm[i] = zm; Zs[i] = zs;
        }
        zlen = n + 1;
    }
    G = grow_zero(G, glen, n + 1);
    Cc = grow_zero(Cc, glen, n + 1);
    for (m = glen; m <= n; m++) {
        idx_t N = 2 * m;
        seq_t X = 0, bin = 1, Y = 1, p6 = 1, S = 0;
        int xs = 0;
        for (d = 0; d <= m; d++) {
            seq_t term = bin * Y * Zm[m - d];
            sadd(&X, &xs, term, Zs[m - d]);
            if (d < m) {
                bin = bin * (seq_t)(N - 2 * d) / (seq_t)(2 * d + 1);
                bin = bin * (seq_t)(N - 2 * d - 1) / (seq_t)(2 * d + 2);
                Y *= (seq_t)(12 * (2 * d + 1));
            }
        }
        for (i = 0; i < N; i++) p6 *= 6;
        G[m] = X / p6;
        if (m == 0) { Cc[m] = 1; continue; }
        bin = (seq_t)(2 * m - 1);
        for (k = 1; k < m; k++) {
            idx_t j = 2 * k - 1;
            S += bin * Cc[k] * G[m - k];
            bin = bin * (seq_t)(2 * m - 1 - j) / (seq_t)(j + 1);
            bin = bin * (seq_t)(2 * m - 2 - j) / (seq_t)(j + 2);
        }
        Cc[m] = G[m] - S;
    }
    glen = n + 1;
}

/* ---------- fixed-width path (exact arithmetic modulo 2^384) ---------- */

#define WL 12
typedef struct { uint32_t w[WL]; } W;

static W wz(uint64_t v)
{
    W r;
    memset(&r, 0, sizeof r);
    r.w[0] = (uint32_t)v;
    r.w[1] = (uint32_t)(v >> 32);
    return r;
}

static W wadd(W a, W b)
{
    W r; uint64_t c = 0; int i;
    for (i = 0; i < WL; i++) {
        c += (uint64_t)a.w[i] + b.w[i];
        r.w[i] = (uint32_t)c;
        c >>= 32;
    }
    return r;
}

static W wsub(W a, W b)
{
    W r; uint64_t br = 0; int i;
    for (i = 0; i < WL; i++) {
        uint64_t t = (uint64_t)a.w[i] - b.w[i] - br;
        r.w[i] = (uint32_t)t;
        br = (t >> 32) & 1;
    }
    return r;
}

static W wmul(W a, W b)
{
    W r; int i, j;
    memset(&r, 0, sizeof r);
    for (i = 0; i < WL; i++) {
        uint64_t c = 0;
        for (j = 0; i + j < WL; j++) {
            c += (uint64_t)a.w[i] * b.w[j] + r.w[i + j];
            r.w[i + j] = (uint32_t)c;
            c >>= 32;
        }
    }
    return r;
}

static W wshr(W a, unsigned k)
{
    W r; unsigned q = k / 32, s = k % 32; unsigned i;
    for (i = 0; i < WL; i++) {
        uint32_t lo = (i + q < WL) ? (a.w[i + q] >> s) : 0;
        uint32_t hi = (s && i + q + 1 < WL) ? (a.w[i + q + 1] << (32 - s)) : 0;
        r.w[i] = lo | hi;
    }
    return r;
}

static seq_t fixed_a(idx_t n)
{
    static W P[64][64];
    static int pinit = 0;
    W T[64], P3[64], Z[32], Gw[32], Cw[32], inv3;
    int N, i, j, m, d, k, M, A;
    if (n > 30) return 0;
    if (!pinit) {
        for (i = 0; i < 64; i++)
            for (j = 0; j < 64; j++) {
                if (j == 0 || j == i) P[i][j] = wz(1);
                else if (j > i) P[i][j] = wz(0);
                else P[i][j] = wadd(P[i - 1][j - 1], P[i - 1][j]);
            }
        pinit = 1;
    }
    N = 2 * (int)n;
    T[0] = wz(1);
    T[1] = wsub(wz(0), wz(1));
    for (i = 2; i <= N; i++)
        T[i] = wsub(wsub(wz(0), T[i - 1]), wmul(wz((uint64_t)(2 * (i - 1))), T[i - 2]));
    P3[0] = wz(1);
    for (i = 1; i <= N; i++) P3[i] = wmul(P3[i - 1], wz(3));
    for (i = 0; 2 * i <= N; i++) {
        W s = wz(0), df = wz(1);
        M = 2 * i;
        for (j = 1; j + 1 <= M; j += 2) df = wmul(df, wz((uint64_t)j));
        for (A = 0; A <= M; A++) {
            s = wadd(s, wmul(wmul(P[M][A], df), wmul(T[M - A], P3[M - A])));
            df = wmul(df, wz((uint64_t)(M + 2 * A + 1)));
        }
        Z[i] = s;
    }
    memset(&inv3, 0xAA, sizeof inv3);
    inv3.w[0] = 0xAAAAAAABu;
    for (m = 0; m <= (int)n; m++) {
        W X = wz(0), Y = wz(1);
        int NN = 2 * m;
        for (d = 0; d <= m; d++) {
            X = wadd(X, wmul(wmul(P[NN][2 * d], Y), Z[m - d]));
            Y = wmul(Y, wz((uint64_t)(12 * (2 * d + 1))));
        }
        X = wshr(X, (unsigned)NN);
        for (i = 0; i < NN; i++) X = wmul(X, inv3);
        Gw[m] = X;
        if (m == 0) { Cw[m] = wz(1); continue; }
        {
            W S = wz(0);
            for (k = 1; k < m; k++)
                S = wadd(S, wmul(wmul(P[2 * m - 1][2 * k - 1], Cw[k]), Gw[m - k]));
            Cw[m] = wsub(Gw[m], S);
        }
    }
    {
        unsigned __int128 v = 0;
        for (i = 3; i >= 0; i--) v = (v << 32) | Cw[n].w[i];
        return u128_to_seq(v);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (!seq_is_big()) return fixed_a(n);
    big_extend(n);
    return Cc[n];
}
