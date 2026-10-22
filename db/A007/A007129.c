/*
 * A007129  Number of unlabeled strength 3 Eulerian graphs with n nodes, 2 of
 *          odd degree.
 * https://oeis.org/A007129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside sum over cycle types lambda of S_n (enumerated by
 *            search): a(n) = Sum_{lambda |- n} (c2 + C(c1,2)) * 2^(2*N - k +
 *            [lambda has an odd part]) / z_lambda, where c_j is the number of
 *            parts equal to j, k is the number of parts, N = Sum_i
 *            floor(L_i/2) + Sum_{i<j} gcd(L_i,L_j) is the number of...
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   binom, gsmall, pw2, seq_is_fixed_width, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/pw2.h"
#include "../utils/u128_to_seq.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/gsmall.h"

typedef unsigned __int128 u128_t;

static bool g_big;
static seq_t g_acc;
static u128_t g_acc128;
static idx_t g_sz[1024], g_ct[1024];
static int g_dep;

static u128_t inv_odd(u128_t x) {
    u128_t y = x;
    int i;
    for (i = 0; i < 7; i++) y *= (u128_t)2 - x * y;
    return y;
}

static void eval_term(long long N, long long k, int odd, idx_t c1, idx_t c2,
                      seq_t w, u128_t zi, long long zv) {
    idx_t f = c2 + c1 * (c1 - (c1 ? 1 : 0)) / 2;
    long long E;
    if (f == 0) return;
    E = 2 * N - k + (odd ? 1 : 0);
    if (g_big) {
        seq_t t = w * (seq_t)f;
        g_acc += t * pw2((idx_t)E);
    } else {
        long long v = 0, s;
        idx_t fo = f;
        while ((fo & 1) == 0) { fo >>= 1; v++; }
        s = E - zv + v;
        if (s < 0 || s >= 128) return;
        g_acc128 += ((u128_t)fo * zi) << s;
    }
}

static void rec(idx_t L, idx_t m, long long N, long long k, int odd,
                seq_t w, u128_t zi, long long zv, idx_t c2) {
    idx_t c, cmax, sumg = 0;
    int d;
    seq_t wp;
    u128_t zic = zi, invL;
    long long zvc = zv, vL = 0;
    idx_t Lo = L;
    if (m == 0) { eval_term(N, k, odd, 0, c2, w, zi, zv); return; }
    for (d = 0; d < g_dep; d++) sumg += g_ct[d] * gsmall(L, g_sz[d]);
    while ((Lo & 1) == 0) { Lo >>= 1; vL++; }
    invL = inv_odd((u128_t)Lo);
    cmax = m / L;
    wp = w;
    for (c = 0; c <= cmax; c++) {
        if (c > 0) {
            idx_t co = c;
            long long vc = 0;
            while ((co & 1) == 0) { co >>= 1; vc++; }
            zic = zic * invL * inv_odd((u128_t)co);
            zvc += vL + vc;
            if (g_big) {
                idx_t i;
                for (i = 1; i < L; i++) wp *= (seq_t)(c * L - i);
            }
        }
        if (L == 1 && c != m) continue;
        {
            long long N2 = N + (long long)(c * (L / 2)) + (long long)(c * (c - (c ? 1 : 0)) / 2 * L)
                           + (long long)(c * sumg);
            long long k2 = k + (long long)c;
            int odd2 = odd || ((L & 1) && c > 0);
            idx_t c2n = (L == 2) ? c : c2;
            idx_t m2 = m - c * L;
            seq_t w2 = wp;
            if (g_big) w2 = wp * binom(m, c * L);
            if (L == 1) {
                eval_term(N2, k2, odd2, c, c2n, w2, zic, zvc);
                continue;
            }
            if (c > 0) { g_sz[g_dep] = L; g_ct[g_dep] = c; g_dep++; }
            {
                idx_t nl = L - 1;
                if (nl > m2) nl = m2;
                rec(nl, m2, N2, k2, odd2, w2, zic, zvc, c2n);
            }
            if (c > 0) g_dep--;
        }
    }
}

seq_t a(idx_t n) {
    seq_t one = 1;
    g_big = !seq_is_fixed_width();
    if (!g_big && n > 16) return (seq_t)0;
    g_acc = 0;
    g_acc128 = 0;
    g_dep = 0;
    rec(n, n, 0, 0, 0, one, (u128_t)1, 0, 0);
    if (g_big) {
        seq_t fact = 1;
        idx_t i;
        for (i = 2; i <= n; i++) fact *= (seq_t)i;
        return g_acc / fact;
    }
    return u128_to_seq(g_acc128);
}
