/*
 * A005142  Number of connected bipartite graphs with n nodes.
 * https://oeis.org/A005142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(2n+1) = A318870(2n+1)/2, a(2n) = (A318870(2n) + F(2n))/2,
 *            where A318870 = inverse EULER transform of B(n) = Sum_{m+k=n}
 *            b(m,k), the number of bicolored graphs, b(m,k) = Sum_{lambda|-m,
 *            mu|-k} 2^(Sum_{i,j} gcd(lambda_i,mu_j))/(z_lambda z_mu); F =
 *            connected bicolored graphs invariant under color...
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 15, u64 n <= 21, u128 n <= 28,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/grow_zero.h"

typedef uint64_t u64;

static u64 Pm, lim2;
static int NN, MM;
static u64 *pw2, *inv, *Bacc, *Tacc, *gser, *wv;
static int *gsum, *gcdt;

static int igcd(int x, int y)
{
    while (y) { int r = x % y; x = y; y = r; }
    return x;
}

static void visit(int m, u64 iz, long long eT, int last, int clast)
{
    int K = NN - m, k, t, s, smax;
    for (t = 1; t <= K; t++) wv[t] = pw2[gsum[t]];
    gser[0] = 1;
    for (k = 1; k <= K; k++) {
        u64 acc = 0;
        for (t = 1; t <= k; t++) {
            acc += wv[t] * gser[k - t];
            if (acc >= lim2) acc -= lim2;
        }
        gser[k] = acc % Pm * inv[k] % Pm;
    }
    for (k = m; k <= K; k++) {
        u64 v = iz * gser[k] % Pm;
        if (k != m) v = 2 * v % Pm;
        Bacc[m + k] = (Bacc[m + k] + v) % Pm;
    }
    Tacc[2 * m] = (Tacc[2 * m] + iz * pw2[eT]) % Pm;
    smax = last < MM - m ? last : MM - m;
    for (s = smax; s >= 1; s--) {
        int c = (s == last) ? clast : 0;
        long long e2 = eT + (s + 1) / 2 + gsum[s];
        u64 iz2 = iz * inv[s] % Pm * inv[c + 1] % Pm;
        int *row = gcdt + (size_t)s * (NN + 1);
        for (t = 1; t <= NN; t++) gsum[t] += row[t];
        visit(m + s, iz2, e2, s, c + 1);
        for (t = 1; t <= NN; t++) gsum[t] -= row[t];
    }
}

static void euler_tr(const u64 *X, int L, u64 *E, u64 *cb)
{
    int n, d, k;
    u64 p = Pm;
    for (n = 1; n <= L; n++) cb[n] = 0;
    for (d = 1; d <= L; d++)
        for (n = d; n <= L; n += d) cb[n] = (cb[n] + (u64)d * X[d]) % p;
    E[0] = 1;
    for (n = 1; n <= L; n++) {
        u64 acc = 0;
        for (k = 1; k <= n; k++) acc = (acc + cb[k] * E[n - k]) % p;
        E[n] = acc * inv[n] % p;
    }
}

static void solve_mod(u64 p, int N, u64 *A)
{
    int M = N / 2, n, k, d, s, t, j;
    long long i, pwn = (long long)M + (long long)M * M / 2 + 4;
    u64 *cc, *C, *F, *Pv, *E1, *E2, *cb;
    Pm = p; NN = N; MM = M; lim2 = 2 * p * p;
    pw2 = (u64 *)malloc(sizeof(u64) * pwn);
    pw2[0] = 1;
    for (i = 1; i < pwn; i++) pw2[i] = pw2[i - 1] * 2 % p;
    inv = (u64 *)malloc(sizeof(u64) * (N + 3));
    inv[0] = 0; inv[1] = 1;
    for (n = 2; n <= N + 2; n++) inv[n] = (p - (p / n) * inv[p % n] % p) % p;
    Bacc = (u64 *)calloc(N + 1, sizeof(u64));
    Tacc = (u64 *)calloc(N + 1, sizeof(u64));
    gser = (u64 *)malloc(sizeof(u64) * (N + 1));
    wv = (u64 *)malloc(sizeof(u64) * (N + 1));
    gsum = (int *)calloc(N + 1, sizeof(int));
    gcdt = (int *)malloc(sizeof(int) * (size_t)(M + 1) * (N + 1));
    for (s = 0; s <= M; s++)
        for (t = 0; t <= N; t++) gcdt[(size_t)s * (N + 1) + t] = (s && t) ? igcd(s, t) : 0;

    visit(0, 1, 0, M, 0);

    cc = (u64 *)calloc(N + 1, sizeof(u64));
    C = (u64 *)calloc(N + 1, sizeof(u64));
    for (n = 1; n <= N; n++) {
        u64 acc = (u64)n % p * Bacc[n] % p;
        for (k = 1; k < n; k++) acc = (acc + p - cc[k] * Bacc[n - k] % p) % p;
        cc[n] = acc;
    }
    for (n = 1; n <= N; n++) {
        u64 acc = cc[n];
        for (d = 1; d < n; d++)
            if (n % d == 0) acc = (acc + p - (u64)d * C[d] % p) % p;
        C[n] = acc * inv[n] % p;
    }

    F = (u64 *)calloc(N + 1, sizeof(u64));
    Pv = (u64 *)calloc(N + 1, sizeof(u64));
    E1 = (u64 *)malloc(sizeof(u64) * (N + 1));
    E2 = (u64 *)malloc(sizeof(u64) * (N + 1));
    cb = (u64 *)malloc(sizeof(u64) * (N + 1));
    A[0] = 1;
    for (n = 1; n <= N; n++) {
        if (n & 1) {
            F[n] = 0;
            A[n] = C[n] * inv[2] % p;
        } else {
            int h = n / 2;
            u64 conv = 0;
            for (j = 1; j <= h; j++) Pv[j] = (A[j] + p - F[j]) % p;
            euler_tr(Pv, h, E2, cb);
            F[n] = 0;
            euler_tr(F, n, E1, cb);
            for (j = 0; j <= h; j++) conv = (conv + E2[j] * E1[n - 2 * j]) % p;
            F[n] = (Tacc[n] + p - conv) % p;
            A[n] = (C[n] + F[n]) % p * inv[2] % p;
        }
    }

    free(cc); free(C); free(F); free(Pv); free(E1); free(E2); free(cb);
    free(pw2); free(inv); free(Bacc); free(Tacc); free(gser); free(wv);
    free(gsum); free(gcdt);
}

static int prime31(u64 c)
{
    u64 d;
    if (c < 2) return 0;
    if (c % 2 == 0) return c == 2;
    for (d = 3; d * d <= c; d += 2)
        if (c % d == 0) return 0;
    return 1;
}

static u64 powmod(u64 b, u64 e, u64 p)
{
    u64 r = 1;
    b %= p;
    while (e) {
        if (e & 1) r = r * b % p;
        b = b * b % p;
        e >>= 1;
    }
    return r;
}

static seq_t *cache = NULL;
static idx_t cached = 0;

seq_t a(idx_t n)
{
    int N, K, i, j, m;
    long long bits;
    u64 *pr, *res, *dig, *ip, c;
    if (n < cached) return cache[n];
    N = (int)n;
    if (N < 4) N = 4;
    bits = (long long)N * N / 4 + 64;
    K = (int)(bits / 30) + 1;
    pr = (u64 *)malloc(sizeof(u64) * K);
    c = 2147483647ULL;
    for (i = 0; i < K; i++) {
        while (!prime31(c)) c -= 2;
        pr[i] = c;
        c -= 2;
    }
    res = (u64 *)malloc(sizeof(u64) * (size_t)K * (N + 1));
    for (i = 0; i < K; i++) solve_mod(pr[i], N, res + (size_t)i * (N + 1));
    dig = (u64 *)malloc(sizeof(u64) * K);
    ip = (u64 *)malloc(sizeof(u64) * K);
    for (i = 0; i < K; i++) {
        u64 pi = pr[i], prod = 1;
        for (j = 0; j < i; j++) prod = prod * (pr[j] % pi) % pi;
        ip[i] = powmod(prod, pi - 2, pi);
    }
    cache = grow_zero(cache, cached, (idx_t)N + 1);
    for (m = 0; m <= N; m++) {
        seq_t x = 0;
        for (i = 0; i < K; i++) {
            u64 pi = pr[i], cur = 0, r = res[(size_t)i * (N + 1) + m];
            for (j = i - 1; j >= 0; j--) cur = (cur * (pr[j] % pi) + dig[j]) % pi;
            dig[i] = (r + pi - cur) % pi * ip[i] % pi;
        }
        for (j = K - 1; j >= 0; j--) x = x * (idx_t)pr[j] + (idx_t)dig[j];
        cache[m] = x;
    }
    cached = (idx_t)N + 1;
    free(pr); free(res); free(dig); free(ip);
    return cache[n];
}
