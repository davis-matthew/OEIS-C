/*
 * A006648  Number of graphs with n nodes, n-1 edges and no isolated vertices.
 * https://oeis.org/A006648
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001433(n) - A001434(n-1) = G(n,n-1) - G(n-1,n-1), where
 *            G(v,e) = number of graphs with v nodes and e edges, computed by
 *            Burnside over the cycle types of S_v: G(v,e) = Sum_{partitions p
 *            of v} [x^e] Prod_{edge orbits O of p} (1 + x^|O|) / z(p);
 *            evaluated modulo several large primes and reco...
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   a6648_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a6648_gcd.h"

typedef unsigned __int128 a6648_u128;

static uint64_t a6648_mulm(uint64_t a, uint64_t b, uint64_t p)
{
    return (uint64_t)((a6648_u128)a * b % p);
}

static uint64_t a6648_powm(uint64_t b, uint64_t e, uint64_t p)
{
    uint64_t r = 1 % p;
    b %= p;
    while (e) {
        if (e & 1) r = a6648_mulm(r, b, p);
        b = a6648_mulm(b, b, p);
        e >>= 1;
    }
    return r;
}

static int a6648_isprime(uint64_t n)
{
    static const uint64_t B[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    uint64_t d;
    int s = 0, i, r;
    if (n < 2) return 0;
    for (i = 0; i < 12; i++)
        if (n % B[i] == 0) return n == B[i];
    d = n - 1;
    while (!(d & 1)) { d >>= 1; s++; }
    for (i = 0; i < 12; i++) {
        uint64_t x = a6648_powm(B[i], d, n);
        int ok = 0;
        if (x == 1 || x == n - 1) continue;
        for (r = 1; r < s; r++) {
            x = a6648_mulm(x, x, n);
            if (x == n - 1) { ok = 1; break; }
        }
        if (!ok) return 0;
    }
    return 1;
}

static uint64_t *g_pr = NULL;
static int g_npr = 0;

static void a6648_need_primes(int K)
{
    uint64_t c;
    if (K <= g_npr) return;
    g_pr = (uint64_t *)realloc(g_pr, (size_t)K * sizeof(uint64_t));
    c = g_npr ? g_pr[g_npr - 1] - 2 : ((uint64_t)1 << 62) - 1;
    while (g_npr < K) {
        if (a6648_isprime(c)) g_pr[g_npr++] = c;
        c -= 2;
    }
}

/* DFS state */
static int K;
static const uint64_t *PR;
static uint64_t *POLY, *W, *INV, *ACC;
static long *DEG, *PV, *PC;
static long M;
static size_t SZ;

/* multiply P by (1 + x^L)^E, truncated at degree M */
static void a6648_mul_orbits(uint64_t *P, long *deg, long L, long E)
{
    long e, i;
    int q;
    if (L > M || E <= 0) return;
    for (e = 0; e < E; e++) {
        long top = *deg + L;
        if (top > M) top = M;
        for (i = top; i >= L; i--) {
            uint64_t *dst = P + (size_t)i * K;
            const uint64_t *src = P + (size_t)(i - L) * K;
            for (q = 0; q < K; q++) {
                uint64_t s = dst[q] + src[q];
                if (s >= PR[q]) s -= PR[q];
                dst[q] = s;
            }
        }
        *deg = top;
    }
}

static void a6648_dfs(int depth, long r, long kmin, int nd);

static void a6648_step(int depth, long r, long k, int nd)
{
    uint64_t *P = POLY + (size_t)depth * SZ, *Q = P + SZ;
    uint64_t *w = W + (size_t)depth * K, *w2 = w + K;
    long deg = DEG[depth], cnew;
    int i, q, nnd = nd;

    memcpy(Q, P, SZ * sizeof(uint64_t));
    for (i = 0; i < nd; i++) {
        long j = PV[i], c = PC[i];
        long g = a6648_gcd(j, k);
        a6648_mul_orbits(Q, &deg, j / g * k, g * c);
    }
    if (k % 2)
        a6648_mul_orbits(Q, &deg, k, (k - 1) / 2);
    else {
        a6648_mul_orbits(Q, &deg, k, k / 2 - 1);
        a6648_mul_orbits(Q, &deg, k / 2, 1);
    }
    if (nd > 0 && PV[nd - 1] == k) {
        cnew = ++PC[nd - 1];
    } else {
        PV[nd] = k;
        PC[nd] = 1;
        cnew = 1;
        nnd = nd + 1;
    }
    for (q = 0; q < K; q++)
        w2[q] = a6648_mulm(a6648_mulm(w[q], INV[(size_t)k * K + q], PR[q]),
                           INV[(size_t)cnew * K + q], PR[q]);
    DEG[depth + 1] = deg;
    a6648_dfs(depth + 1, r - k, k, nnd);
    if (nnd == nd) PC[nd - 1]--;
}

static void a6648_dfs(int depth, long r, long kmin, int nd)
{
    long k, half;
    if (r == 0) {
        const uint64_t *P = POLY + (size_t)depth * SZ + (size_t)M * K;
        const uint64_t *w = W + (size_t)depth * K;
        int q;
        for (q = 0; q < K; q++)
            ACC[q] = (ACC[q] + a6648_mulm(w[q], P[q], PR[q])) % PR[q];
        return;
    }
    half = r / 2;
    for (k = kmin; k <= half; k++)
        a6648_step(depth, r, k, nd);
    if (r >= kmin)
        a6648_step(depth, r, r, nd);
}

static void a6648_run(long V)
{
    int q;
    memset(POLY, 0, SZ * sizeof(uint64_t));
    for (q = 0; q < K; q++) {
        POLY[q] = 1;
        W[q] = 1;
        ACC[q] = 0;
    }
    DEG[0] = 0;
    a6648_dfs(0, V, 1, 0);
}

seq_t a(idx_t n)
{
    long V = (long)n, N, bits, t;
    int bl = 0, q, i, j;
    uint64_t *R1, *res, *v;
    seq_t X;

    M = V - 1;
    N = V * (V - 1) / 2;
    while ((N >> bl) != 0) bl++;
    bits = M * bl + 2;
    K = (int)(bits / 61 + 2);
    a6648_need_primes(K);
    PR = g_pr;

    SZ = (size_t)(M + 1) * K;
    POLY = (uint64_t *)malloc((size_t)(V + 2) * SZ * sizeof(uint64_t));
    W = (uint64_t *)malloc((size_t)(V + 2) * K * sizeof(uint64_t));
    INV = (uint64_t *)malloc((size_t)(V + 2) * K * sizeof(uint64_t));
    ACC = (uint64_t *)malloc((size_t)K * sizeof(uint64_t));
    R1 = (uint64_t *)malloc((size_t)K * sizeof(uint64_t));
    res = (uint64_t *)malloc((size_t)K * sizeof(uint64_t));
    v = (uint64_t *)malloc((size_t)K * sizeof(uint64_t));
    DEG = (long *)malloc((size_t)(V + 2) * sizeof(long));
    PV = (long *)malloc((size_t)(V + 2) * sizeof(long));
    PC = (long *)malloc((size_t)(V + 2) * sizeof(long));

    for (t = 1; t <= V; t++)
        for (q = 0; q < K; q++)
            INV[(size_t)t * K + q] = a6648_powm((uint64_t)t, PR[q] - 2, PR[q]);

    a6648_run(V);
    for (q = 0; q < K; q++) R1[q] = ACC[q];
    a6648_run(V - 1);
    for (q = 0; q < K; q++)
        res[q] = (R1[q] + PR[q] - ACC[q]) % PR[q];

    /* Garner mixed-radix reconstruction */
    for (i = 0; i < K; i++) {
        uint64_t p = PR[i], acc = 0, coef = 1 % p, x;
        for (j = 0; j < i; j++) {
            acc = (acc + a6648_mulm(v[j] % p, coef, p)) % p;
            coef = a6648_mulm(coef, PR[j] % p, p);
        }
        x = (res[i] + p - acc) % p;
        v[i] = a6648_mulm(x, a6648_powm(coef, p - 2, p), p);
    }
    X = (seq_t)v[K - 1];
    for (i = K - 2; i >= 0; i--)
        X = X * (seq_t)PR[i] + (seq_t)v[i];

    free(POLY); free(W); free(INV); free(ACC); free(R1); free(res); free(v);
    free(DEG); free(PV); free(PC);
    return X;
}
