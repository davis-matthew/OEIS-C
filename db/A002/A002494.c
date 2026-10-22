/*
 * A002494  Number of n-node graphs without isolated nodes.
 * https://oeis.org/A002494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = A000088(n) - A000088(n-1) for n >= 1, a(0) = 1, where
 *            A000088(n) = Sum_{partitions lambda of n} 2^{c(lambda)} /
 *            z(lambda) (cycle index of the pair group: c = number of edge
 *            orbits). It is evaluated mod many 62-bit primes with Montgomery
 *            arithmetic and combined by CRT. Cycles of length 1 an...
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 11, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   a2494_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a2494_gcd.h"

typedef unsigned long long a2494_u64;

static a2494_u64 a2494_mulm(a2494_u64 x, a2494_u64 y, a2494_u64 p) {
    return (a2494_u64)((unsigned __int128)x * y % p);
}

static a2494_u64 a2494_powm(a2494_u64 b, a2494_u64 e, a2494_u64 p) {
    a2494_u64 r = 1 % p;
    b %= p;
    while (e) {
        if (e & 1) r = a2494_mulm(r, b, p);
        b = a2494_mulm(b, b, p);
        e >>= 1;
    }
    return r;
}

static int a2494_isprime(a2494_u64 n) {
    static const a2494_u64 bases[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    a2494_u64 d;
    int s, i, j;
    if (n < 2) return 0;
    for (i = 0; i < 12; i++) {
        if (n == bases[i]) return 1;
        if (n % bases[i] == 0) return 0;
    }
    d = n - 1;
    s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (i = 0; i < 12; i++) {
        a2494_u64 x = a2494_powm(bases[i], d, n);
        if (x == 1 || x == n - 1) continue;
        for (j = 1; j < s; j++) {
            x = a2494_mulm(x, x, n);
            if (x == n - 1) break;
        }
        if (j >= s) return 0;
    }
    return 1;
}

/* Montgomery product, R = 2^64, inputs < p < 2^62 */
static inline a2494_u64 a2494_mm(a2494_u64 a, a2494_u64 b, a2494_u64 p, a2494_u64 ni) {
    unsigned __int128 t = (unsigned __int128)a * b;
    a2494_u64 m = (a2494_u64)t * ni;
    a2494_u64 r = (a2494_u64)((t + (unsigned __int128)m * p) >> 64);
    if (r >= p) r -= p;
    return r;
}

static int a2494_K;
static int a2494_T1;
static a2494_u64 *a2494_P;
static a2494_u64 *a2494_NI;
static a2494_u64 *a2494_invk;   /* [k*K + i], Montgomery form */
static a2494_u64 *a2494_pw;     /* [c*K + i], Montgomery form of 2^c */
static a2494_u64 *a2494_D;      /* [((r*(T1+1)+tot)*(T1+1)+e)*K + i] */
static a2494_u64 *a2494_buf;
static a2494_u64 *a2494_S;
static int *a2494_part;
static int *a2494_mult;

static void a2494_rec(int k, int r, long long c, int lvl, int d, int tot, int e) {
    int K = a2494_K, i, m, t;
    const a2494_u64 *parent = a2494_buf + (size_t)lvl * K;
    if (k > r) k = r;
    if (k <= 2) {
        size_t T1 = (size_t)a2494_T1 + 1;
        const a2494_u64 *dv = a2494_D + (((size_t)r * T1 + tot) * T1 + e) * K;
        const a2494_u64 *pv = a2494_pw + (size_t)c * K;
        for (i = 0; i < K; i++) {
            a2494_u64 p = a2494_P[i], ni = a2494_NI[i];
            a2494_u64 x = a2494_mm(a2494_mm(parent[i], pv[i], p, ni), dv[i], p, ni);
            a2494_u64 s = a2494_S[i] + x;
            if (s >= p) s -= p;
            a2494_S[i] = s;
        }
        return;
    }
    a2494_rec(k - 1, r, c, lvl, d, tot, e);
    {
        long long g = 0;
        a2494_u64 *cur = a2494_buf + (size_t)(lvl + 1) * K;
        const a2494_u64 *ik = a2494_invk + (size_t)k * K;
        for (t = 0; t < d; t++) g += (long long)a2494_mult[t] * a2494_gcd(k, a2494_part[t]);
        for (m = 1; (long long)m * k <= r; m++) {
            long long cm;
            if (m == 1) {
                for (i = 0; i < K; i++)
                    cur[i] = a2494_mm(parent[i], ik[i], a2494_P[i], a2494_NI[i]);
            } else {
                const a2494_u64 *im = a2494_invk + (size_t)m * K;
                for (i = 0; i < K; i++) {
                    a2494_u64 p = a2494_P[i], ni = a2494_NI[i];
                    cur[i] = a2494_mm(a2494_mm(cur[i], ik[i], p, ni), im[i], p, ni);
                }
            }
            cm = c + (long long)m * (k / 2) + (long long)k * m * (m - 1) / 2 + (long long)m * g;
            a2494_part[d] = k;
            a2494_mult[d] = m;
            a2494_rec(k - 1, r - m * k, cm, lvl + 1, d + 1, tot + m, e + ((k % 2 == 0) ? m : 0));
        }
    }
}

seq_t a(idx_t n) {
    int N, K, T1, i, j, r, tot, e, mm2;
    long long C;
    size_t T1s;
    a2494_u64 q, *R, *X, *ONE, *invf, *h;
    seq_t res, t;
    int small;

    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    N = (int)n;
    C = (long long)N * (N - 1) / 2;
    K = (int)(C / 61) + 2;
    T1 = N / 3;
    T1s = (size_t)T1 + 1;

    a2494_K = K;
    a2494_T1 = T1;
    a2494_P = (a2494_u64 *)malloc(sizeof(a2494_u64) * K);
    a2494_NI = (a2494_u64 *)malloc(sizeof(a2494_u64) * K);
    ONE = (a2494_u64 *)malloc(sizeof(a2494_u64) * K);
    q = (1ULL << 62) - 1;
    for (i = 0; i < K; i++) {
        a2494_u64 p, x;
        while (!a2494_isprime(q)) q -= 2;
        p = q;
        a2494_P[i] = p;
        q -= 2;
        x = p;
        for (j = 0; j < 6; j++) x *= 2 - p * x;
        a2494_NI[i] = (a2494_u64)0 - x;
        ONE[i] = ((a2494_u64)0 - p) % p;
    }

    a2494_invk = (a2494_u64 *)malloc(sizeof(a2494_u64) * (size_t)(N + 1) * K);
    invf = (a2494_u64 *)malloc(sizeof(a2494_u64) * (size_t)(N + 1) * K);
    h = (a2494_u64 *)malloc(sizeof(a2494_u64) * (size_t)(N / 2 + 1) * K);
    a2494_pw = (a2494_u64 *)malloc(sizeof(a2494_u64) * (size_t)(C + 1) * K);
    for (i = 0; i < K; i++) {
        a2494_u64 p = a2494_P[i], ni = a2494_NI[i];
        a2494_u64 R2 = a2494_mulm(ONE[i], ONE[i], p);
        long long cc;
        a2494_invk[i] = 0;
        invf[i] = ONE[i];
        for (j = 1; j <= N; j++) {
            a2494_u64 v = a2494_powm((a2494_u64)j, p - 2, p);
            a2494_invk[(size_t)j * K + i] = a2494_mm(v, R2, p, ni);
            invf[(size_t)j * K + i] = a2494_mm(invf[(size_t)(j - 1) * K + i],
                                               a2494_invk[(size_t)j * K + i], p, ni);
        }
        h[i] = ONE[i];
        for (j = 1; j <= N / 2; j++)
            h[(size_t)j * K + i] = a2494_mm(h[(size_t)(j - 1) * K + i],
                                            a2494_mm(a2494_invk[(size_t)2 * K + i],
                                                     a2494_invk[(size_t)j * K + i], p, ni), p, ni);
        a2494_pw[i] = ONE[i];
        for (cc = 1; cc <= C; cc++) {
            a2494_u64 v = a2494_pw[(size_t)(cc - 1) * K + i] << 1;
            if (v >= p) v -= p;
            a2494_pw[(size_t)cc * K + i] = v;
        }
    }

    /* table of sums over 2-cycles and fixed points */
    a2494_D = (a2494_u64 *)calloc((size_t)(N + 1) * T1s * T1s * K, sizeof(a2494_u64));
    for (r = 0; r <= N; r++) {
        for (tot = 0; tot <= (N - r) / 3; tot++) {
            for (e = 0; e <= tot; e++) {
                a2494_u64 *dv = a2494_D + (((size_t)r * T1s + tot) * T1s + e) * K;
                for (mm2 = 0; 2 * mm2 <= r; mm2++) {
                    long long m = mm2, f = r - 2 * mm2;
                    long long ex = m + m * (m - 1) + m * (tot + e) + f * (f - 1) / 2 + f * (tot + m);
                    const a2494_u64 *pv = a2494_pw + (size_t)ex * K;
                    const a2494_u64 *hv = h + (size_t)m * K;
                    const a2494_u64 *fv = invf + (size_t)f * K;
                    for (i = 0; i < K; i++) {
                        a2494_u64 p = a2494_P[i], ni = a2494_NI[i];
                        a2494_u64 x = a2494_mm(a2494_mm(pv[i], hv[i], p, ni), fv[i], p, ni);
                        a2494_u64 s = dv[i] + x;
                        if (s >= p) s -= p;
                        dv[i] = s;
                    }
                }
            }
        }
    }
    for (r = N; r >= 1; r--) {
        for (tot = 0; tot <= (N - r) / 3; tot++) {
            for (e = 0; e <= tot; e++) {
                a2494_u64 *dv = a2494_D + (((size_t)r * T1s + tot) * T1s + e) * K;
                const a2494_u64 *dw = a2494_D + (((size_t)(r - 1) * T1s + tot) * T1s + e) * K;
                for (i = 0; i < K; i++) {
                    a2494_u64 p = a2494_P[i];
                    dv[i] = (dv[i] >= dw[i]) ? dv[i] - dw[i] : dv[i] + p - dw[i];
                }
            }
        }
    }

    a2494_buf = (a2494_u64 *)malloc(sizeof(a2494_u64) * (size_t)(N + 2) * K);
    a2494_S = (a2494_u64 *)malloc(sizeof(a2494_u64) * K);
    a2494_part = (int *)malloc(sizeof(int) * (N + 2));
    a2494_mult = (int *)malloc(sizeof(int) * (N + 2));
    R = (a2494_u64 *)malloc(sizeof(a2494_u64) * K);
    X = (a2494_u64 *)malloc(sizeof(a2494_u64) * K);

    for (i = 0; i < K; i++) { a2494_buf[i] = ONE[i]; a2494_S[i] = 0; }
    a2494_rec(N, N, 0, 0, 0, 0, 0);
    for (i = 0; i < K; i++) R[i] = a2494_mm(a2494_S[i], 1, a2494_P[i], a2494_NI[i]);

    /* Garner mixed-radix digits */
    for (i = 0; i < K; i++) {
        a2494_u64 p = a2494_P[i], tv = 0, prod = 1 % p, diff;
        for (j = i - 1; j >= 0; j--) {
            tv = (a2494_mulm(tv, a2494_P[j] % p, p) + X[j] % p) % p;
        }
        for (j = 0; j < i; j++) prod = a2494_mulm(prod, a2494_P[j] % p, p);
        diff = (R[i] % p + p - tv) % p;
        X[i] = a2494_mulm(diff, a2494_powm(prod, p - 2, p), p);
    }

    t = (seq_t)1;
    for (i = 0; i < 64; i++) t += t;
    small = (t == 0);
    if (small) {
        a2494_u64 u = 0;
        for (i = K - 1; i >= 0; i--) u = u * a2494_P[i] + X[i];
        res = (seq_t)u;
    } else {
        res = (seq_t)0;
        for (i = K - 1; i >= 0; i--) res = res * (seq_t)a2494_P[i] + (seq_t)X[i];
    }

    free(a2494_P); free(a2494_NI); free(ONE); free(a2494_invk); free(invf); free(h);
    free(a2494_pw); free(a2494_D); free(a2494_buf); free(a2494_S);
    free(a2494_part); free(a2494_mult);
    free(R); free(X);
    return res;
}
