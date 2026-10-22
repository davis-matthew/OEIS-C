/*
 * A000083  Number of mixed Husimi trees with n nodes; or polygonal cacti with
 *          bridges.
 * https://oeis.org/A000083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   G.f. A(x) = B(x) + C(x) - B(x)*D(x), with B = x*Product_{k>=1}
 *            (1-x^k)^(-d(k)) (A000237, rooted, via Euler transform), D = B +
 *            (B^2/(1-B) + (1+B)*B(x^2)/(1-B(x^2)))/2 (A035350), C = (2*L -
 *            2*B + (1+B)^2*T + B^2)/4 (A035349), where T = B(x^2)/(1-B(x^2)),
 *            L = Sum_{d>=1} (phi(d)/d) * (-log(1 - B(x^d...
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 21, u64 n <= 36, u128 n <= 67,
 *            big any n
 * Helpers:   alloc_series, madd, minv, mmul, mpow, msub, totient_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/alloc_series.h"
#include "../utils/madd.h"
#include "../utils/minv.h"
#include "../utils/mmul.h"
#include "../utils/mpow.h"
#include "../utils/msub.h"
#include "../utils/totient_small.h"

static int big_mode = -1;

static bool is_big(void)
{
    if (big_mode < 0) {
        seq_t x = 1;
        int i;
        for (i = 0; i < 200; i++) x <<= 1;
        big_mode = (x != 0) ? 1 : 0;
    }
    return big_mode == 1;
}

/* ---------- arbitrary-precision path (seq_t) ---------- */
static seq_t *Bb, *Bs, *Bt, *Bbt, *Bf, *Bu, *Bh, *Bc, *Bp;
static long bcap = -1;

static void big_build(long N)
{
    long m, i, d;
    Bb = alloc_series(N + 1); Bs = alloc_series(N + 1); Bt = alloc_series(N + 1);
    Bbt = alloc_series(N + 1); Bf = alloc_series(N + 1); Bu = alloc_series(N + 1);
    Bh = alloc_series(N + 1); Bc = alloc_series(N + 1); Bp = alloc_series(N + 1);
    Bb[0] = 0; Bs[0] = 0; Bt[0] = 0; Bbt[0] = 0; Bf[0] = 0;
    Bu[0] = 0; Bh[0] = 0; Bc[0] = 0; Bp[0] = 1;
    for (m = 1; m <= N; m++) {
        seq_t acc;
        Bb[m] = Bp[m - 1];
        acc = Bb[m];
        for (i = 1; i < m; i++) acc += Bb[i] * Bs[m - i];
        Bs[m] = acc;
        if (m % 2 == 0) {
            acc = Bb[m / 2];
            for (i = 1; 2 * i < m; i++) acc += Bb[i] * Bt[m - 2 * i];
            Bt[m] = acc;
        } else {
            Bt[m] = 0;
        }
        acc = 0;
        for (i = 1; i < m; i++) acc += Bb[i] * Bt[m - i];
        Bbt[m] = acc;
        Bf[m] = Bb[m] + (Bs[m] - Bb[m] + Bt[m] + Bbt[m]) / 2;
        acc = 0;
        for (i = 1; i < m; i++) acc += Bb[i] * Bb[m - i];
        Bu[m] = acc;
        acc = (seq_t)m * Bb[m];
        for (i = 1; i < m; i++) acc += (seq_t)i * Bb[i] * Bs[m - i];
        Bh[m] = acc;
        acc = 0;
        for (d = 1; d * d <= m; d++)
            if (m % d == 0) {
                long e = m / d;
                acc += (seq_t)d * Bf[d];
                if (e != d) acc += (seq_t)e * Bf[e];
            }
        Bc[m] = acc;
        acc = 0;
        for (i = 1; i <= m; i++) acc += Bc[i] * Bp[m - i];
        Bp[m] = acc / m;
    }
    bcap = N;
}

static seq_t big_a(long n)
{
    seq_t L = 0, b2t = 0, bf = 0, num;
    long d, i;
    if (n > bcap) {
        long N = 2 * bcap;
        if (N < n) N = n;
        if (N < 32) N = 32;
        big_build(N);
    }
    for (d = 1; d <= n; d++)
        if (n % d == 0) L += (seq_t)totient_small((unsigned long long)d) * Bh[n / d];
    L /= n;
    for (i = 1; i < n; i++) {
        b2t += Bu[i] * Bt[n - i];
        bf += Bb[i] * Bf[n - i];
    }
    num = 2 * L + Bt[n] + 2 * Bbt[n] + b2t + Bu[n] - 2 * Bb[n];
    return Bb[n] + num / 4 - bf;
}

/* ---------- fixed-width path: three primes + CRT ---------- */
#define MC 200
static const uint64_t PR[3] = {2305843009213693951ULL, 4611686018427387847ULL,
                               9223372036854775783ULL};
static uint64_t Mb[3][MC + 1], Ms[3][MC + 1], Mt[3][MC + 1], Mbt[3][MC + 1],
    Mf[3][MC + 1], Mu[3][MC + 1], Mh[3][MC + 1], Mcc[3][MC + 1], Mp[3][MC + 1];
static bool mod_built = false;

static void mod_build(int k)
{
    uint64_t p = PR[k], inv2 = (p + 1) / 2;
    uint64_t *b = Mb[k], *s = Ms[k], *t = Mt[k], *bt = Mbt[k], *f = Mf[k];
    uint64_t *u = Mu[k], *h = Mh[k], *c = Mcc[k], *pp = Mp[k];
    long m, i, d;
    b[0] = s[0] = t[0] = bt[0] = f[0] = u[0] = h[0] = c[0] = 0;
    pp[0] = 1;
    for (m = 1; m <= MC; m++) {
        uint64_t acc;
        b[m] = pp[m - 1];
        acc = b[m];
        for (i = 1; i < m; i++) acc = madd(acc, mmul(b[i], s[m - i], p), p);
        s[m] = acc;
        if (m % 2 == 0) {
            acc = b[m / 2];
            for (i = 1; 2 * i < m; i++) acc = madd(acc, mmul(b[i], t[m - 2 * i], p), p);
            t[m] = acc;
        } else {
            t[m] = 0;
        }
        acc = 0;
        for (i = 1; i < m; i++) acc = madd(acc, mmul(b[i], t[m - i], p), p);
        bt[m] = acc;
        acc = madd(madd(msub(s[m], b[m], p), t[m], p), bt[m], p);
        f[m] = madd(b[m], mmul(acc, inv2, p), p);
        acc = 0;
        for (i = 1; i < m; i++) acc = madd(acc, mmul(b[i], b[m - i], p), p);
        u[m] = acc;
        acc = mmul((uint64_t)m, b[m], p);
        for (i = 1; i < m; i++)
            acc = madd(acc, mmul(mmul((uint64_t)i, b[i], p), s[m - i], p), p);
        h[m] = acc;
        acc = 0;
        for (d = 1; d * d <= m; d++)
            if (m % d == 0) {
                long e = m / d;
                acc = madd(acc, mmul((uint64_t)d, f[d], p), p);
                if (e != d) acc = madd(acc, mmul((uint64_t)e, f[e], p), p);
            }
        c[m] = acc;
        acc = 0;
        for (i = 1; i <= m; i++) acc = madd(acc, mmul(c[i], pp[m - i], p), p);
        pp[m] = mmul(acc, minv((uint64_t)m, p), p);
    }
}

static uint64_t mod_a(int k, long n)
{
    uint64_t p = PR[k], L = 0, b2t = 0, bf = 0, num, cc;
    uint64_t *b = Mb[k], *t = Mt[k], *bt = Mbt[k], *f = Mf[k], *u = Mu[k], *h = Mh[k];
    long d, i;
    for (d = 1; d <= n; d++)
        if (n % d == 0)
            L = madd(L, mmul(totient_small((unsigned long long)d) % p, h[n / d], p), p);
    L = mmul(L, minv((uint64_t)n, p), p);
    for (i = 1; i < n; i++) {
        b2t = madd(b2t, mmul(u[i], t[n - i], p), p);
        bf = madd(bf, mmul(b[i], f[n - i], p), p);
    }
    num = madd(L, L, p);
    num = madd(num, t[n], p);
    num = madd(num, madd(bt[n], bt[n], p), p);
    num = madd(num, b2t, p);
    num = madd(num, u[n], p);
    num = msub(num, madd(b[n], b[n], p), p);
    cc = mmul(num, minv(4, p), p);
    return msub(madd(b[n], cc, p), bf, p);
}

static seq_t fixed_a(long n)
{
    uint64_t p1 = PR[0], p2 = PR[1], p3 = PR[2];
    uint64_t r1, r2, r3, d1, d2, d3, x12;
    unsigned __int128 x;
    seq_t r;
    int i;
    if (!mod_built) {
        mod_build(0); mod_build(1); mod_build(2);
        mod_built = true;
    }
    r1 = mod_a(0, n); r2 = mod_a(1, n); r3 = mod_a(2, n);
    d1 = r1;
    d2 = mmul(msub(r2, d1 % p2, p2), minv(p1 % p2, p2), p2);
    x12 = madd(d1 % p3, mmul(d2 % p3, p1 % p3, p3), p3);
    d3 = mmul(msub(r3, x12, p3), minv(mmul(p1 % p3, p2 % p3, p3), p3), p3);
    x = (unsigned __int128)d1 + (unsigned __int128)d2 * p1
        + (unsigned __int128)d3 * ((unsigned __int128)p1 * p2);
    r = (seq_t)(uint64_t)(x >> 64);
    for (i = 0; i < 64; i++) r <<= 1;
    r += (seq_t)(uint64_t)x;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (is_big()) return big_a((long)n);
    if (n > MC) return 0; /* far beyond 128 bits */
    return fixed_a((long)n);
}
