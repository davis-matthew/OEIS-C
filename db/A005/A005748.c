/*
 * A005748  Number of n-covers of an unlabeled 7-set.
 * https://oeis.org/A005748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = U(n,7) - U(n,6), where U(n,m) = number of n X m binary
 *            matrices up to row and column permutations = Sum_{lambda |- m}
 *            (1/z_lambda) [x^n] Prod_{d>=1} (1-x^d)^(-o_d(lambda))
 *            (Burnside), o_d(lambda) = number of orbits of length d of a
 *            permutation of cycle type lambda acting on {0,1}^m
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 45,
 *            big any n
 * Helpers:   alloc_zero, gcd_i, release, seq_is_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_big.h"
#include "../utils/u128_to_seq.h"
#include "../utils/gcd_i.h"

typedef unsigned __int128 u128;

#define MAXD 12

static int np = 0;
static int pm[32];
static long long pw[32];
static long long po[32][MAXD + 1];

static void store_part(int m, const int *cur, int k) {
    long long z = 1, fix[MAXD + 1], fact = 1;
    int cnt[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int i, d, e;
    for (i = 0; i < k; i++) cnt[cur[i]]++;
    for (i = 1; i <= 7; i++) {
        int j;
        for (j = 1; j <= cnt[i]; j++) z *= (long long)i * j;
    }
    for (i = 2; i <= m; i++) fact *= i;
    pm[np] = m;
    pw[np] = (m == 7) ? fact / z : 7 * (fact / z);
    for (e = 1; e <= MAXD; e++) {
        int s = 0;
        for (i = 0; i < k; i++) s += gcd_i(cur[i], e);
        fix[e] = 1LL << s;
    }
    for (d = 1; d <= MAXD; d++) {
        long long v = fix[d];
        for (e = 1; e < d; e++)
            if (d % e == 0) v -= e * po[np][e];
        po[np][d] = v / d;
    }
    np++;
}

static void gen(int m, int rem, int maxp, int *cur, int k) {
    int p;
    if (rem == 0) { store_part(m, cur, k); return; }
    for (p = (rem < maxp ? rem : maxp); p >= 1; p--) {
        cur[k] = p;
        gen(m, rem - p, p, cur, k + 1);
    }
}

static void init_parts(void) {
    int cur[8];
    if (np) return;
    gen(7, 7, 7, cur, 0);
    gen(6, 6, 6, cur, 0);
}

/* ---- arbitrary precision path ---- */
static seq_t *Bres = 0;
static idx_t Bn = 0;

static void compute_big(idx_t N) {
    seq_t *T7 = alloc_zero(N + 1), *T6 = alloc_zero(N + 1), *c = alloc_zero(N + 1);
    int p, d;
    idx_t k;
    long long t;
    for (p = 0; p < np; p++) {
        idx_t o1 = (idx_t)po[p][1];
        seq_t *T = (pm[p] == 7) ? T7 : T6;
        c[0] = 1;
        for (k = 1; k <= N; k++)
            c[k] = c[k - 1] * (seq_t)(o1 - 1 + k) / (seq_t)k;
        for (d = 2; d <= MAXD; d++)
            for (t = 0; t < po[p][d]; t++)
                for (k = (idx_t)d; k <= N; k++)
                    c[k] += c[k - d];
        for (k = 0; k <= N; k++)
            T[k] += c[k] * (seq_t)pw[p];
    }
    if (Bres) release(Bres, Bn + 1);
    Bres = alloc_zero(N + 1);
    for (k = 0; k <= N; k++)
        Bres[k] = (T7[k] - T6[k]) / (seq_t)5040;
    release(T7, N + 1);
    release(T6, N + 1);
    release(c, N + 1);
    Bn = N;
}

/* ---- fixed width path: values mod 2^132 as (lo, hi) ---- */
typedef struct { u128 lo; unsigned hi; } W;

static void w_add(W *x, const W *y) {
    u128 s = x->lo + y->lo;
    x->hi += y->hi + (s < y->lo ? 1u : 0u);
    x->lo = s;
}

static W w_mul(const W *x, unsigned long long K) {
    W r;
    u128 l = x->lo & (u128)0xFFFFFFFFFFFFFFFFULL, h = x->lo >> 64;
    u128 pl = l * K, ph = h * K;
    r.lo = pl + (ph << 64);
    r.hi = x->hi * (unsigned)K + (unsigned)(ph >> 64) + (r.lo < pl ? 1u : 0u);
    return r;
}

static u128 *Fres = 0;
static idx_t Fn = 0;

static void compute_fixed(idx_t N) {
    W *T7 = (W *)calloc(N + 1, sizeof(W));
    W *T6 = (W *)calloc(N + 1, sizeof(W));
    W *c = (W *)malloc((N + 1) * sizeof(W));
    u128 inv = 315;
    int p, d, it;
    idx_t k;
    long long t;
    for (it = 0; it < 8; it++) inv = inv * ((u128)2 - (u128)315 * inv);
    for (p = 0; p < np; p++) {
        W *T = (pm[p] == 7) ? T7 : T6;
        for (k = 0; k <= N; k++) { c[k].lo = 0; c[k].hi = 0; }
        c[0].lo = 1;
        for (d = 1; d <= MAXD; d++)
            for (t = 0; t < po[p][d]; t++)
                for (k = (idx_t)d; k <= N; k++)
                    w_add(&c[k], &c[k - d]);
        for (k = 0; k <= N; k++) {
            W v = w_mul(&c[k], (unsigned long long)pw[p]);
            w_add(&T[k], &v);
        }
    }
    free(Fres);
    Fres = (u128 *)malloc((N + 1) * sizeof(u128));
    for (k = 0; k <= N; k++) {
        u128 lo = T7[k].lo - T6[k].lo;
        unsigned hi = T7[k].hi - T6[k].hi - (T7[k].lo < T6[k].lo ? 1u : 0u);
        u128 s4 = (lo >> 4) | ((u128)hi << 124);
        Fres[k] = s4 * inv;
    }
    free(T7);
    free(T6);
    free(c);
    Fn = N;
}

seq_t a(idx_t n) {
    init_parts();
    if (seq_is_big()) {
        if (Bres == 0 || n > Bn) {
            idx_t N = Bn + Bn / 2;
            if (N < n) N = n;
            compute_big(N);
        }
        return Bres[n];
    } else {
        if (Fres == 0 || n > Fn) {
            idx_t N = Fn + Fn / 2;
            if (N < n) N = n;
            compute_fixed(N);
        }
        return u128_to_seq(Fres[n]);
    }
}
