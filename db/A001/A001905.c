/*
 * A001905  From higher-order Bernoulli numbers: absolute value of numerator
 *          of D-number D2n(2n-1).
 * https://oeis.org/A001905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = |numerator(D_{2n}^{(2n-1)})| = numerator of
 *            (2n-1)*(2n)!*u(n), where x/arcsin(x) = 1 - Sum_{m>=1}
 *            u(m)*x^(2m). Equivalently a(n) = numerator((2n-1)*V(n)/(3n)!)
 *            with integers V(m) = H(m) - Sum_{i=1..m-1}
 *            C(2m,2i)*C(3m,3i)*V(i)*H(m-i), H(k) = ((2k-1)!!)^2 *
 *            (3k)!/(2k+1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   a1905_isbig, a1905_isprime, a1905_leg
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/a1905_isbig.h"
#include "../utils/a1905_isprime.h"
#include "../utils/a1905_leg.h"

typedef unsigned __int128 a1905_u128;

#define A1905_LIM 10002

static seq_t a1905_V[A1905_LIM];
static seq_t a1905_H[A1905_LIM];
static idx_t a1905_cnt = 0;
static seq_t a1905_df = 1;   /* (2k-1)!! for k = a1905_cnt */
static seq_t a1905_fc = 1;   /* (3k)!    for k = a1905_cnt */

static a1905_u128 a1905_inv(a1905_u128 q) {
    a1905_u128 x = q;
    int i;
    for (i = 0; i < 8; i++) x *= (a1905_u128)2 - q * x;
    return x;
}

static uint64_t a1905_mm(uint64_t a, uint64_t b, uint64_t M) {
    return (uint64_t)(((a1905_u128)a * b) % M);
}

static seq_t a1905_from128(a1905_u128 A) {
    seq_t res = 0;
    int b;
    for (b = 15; b >= 0; b--) {
        res = (seq_t)(res << 8);
        res = (seq_t)(res + (seq_t)(unsigned)((A >> (8 * b)) & 0xFF));
    }
    return res;
}

static a1905_u128 a1905_C[73][73];

static seq_t a1905_fixed(idx_t n) {
    int N, K, i, j, k, m;
    a1905_u128 G[25], U[25], dd, r, B, s;
    uint64_t p;
    if (n > 24) return 0; /* cannot fit in 128 bits */
    N = (int)n;
    K = 3 * N;
    for (i = 0; i <= K; i++) {
        a1905_C[i][0] = 1;
        a1905_C[i][i] = 1;
        for (j = 1; j < i; j++) a1905_C[i][j] = a1905_C[i - 1][j - 1] + a1905_C[i - 1][j];
    }
    /* 2-adic part: (2n-1)*U(n) mod 2^128, U(m) = (2m)! u(m) */
    dd = 1;
    for (k = 1; k <= N; k++) {
        dd *= (a1905_u128)(2 * k - 1);
        G[k] = dd * dd * a1905_inv((a1905_u128)(2 * k + 1));
    }
    for (m = 1; m <= N; m++) {
        s = 0;
        for (i = 1; i < m; i++) s += a1905_C[2 * m][2 * i] * U[i] * G[m - i];
        U[m] = G[m] - s;
    }
    r = (a1905_u128)(2 * N - 1) * U[N];
    /* odd denominator B via V(n) mod p^v_p((3n)!) */
    B = 1;
    for (p = 3; p <= (uint64_t)K; p += 2) {
        uint64_t vQ, M, e, v, T, Hm[25], Vm[25];
        if (!a1905_isprime(p)) continue;
        vQ = a1905_leg((uint64_t)K, p);
        M = 1;
        for (e = 0; e < vQ; e++) M *= p;
        for (k = 1; k <= N; k++) {
            uint64_t h = 1 % M, d = 1 % M;
            for (j = 1; j <= 3 * k; j++)
                if (j != 2 * k + 1) h = a1905_mm(h, (uint64_t)j % M, M);
            for (j = 1; j <= 2 * k - 1; j += 2) d = a1905_mm(d, (uint64_t)j % M, M);
            h = a1905_mm(a1905_mm(h, d, M), d, M);
            Hm[k] = h;
        }
        for (m = 1; m <= N; m++) {
            uint64_t sm = 0;
            for (i = 1; i < m; i++) {
                uint64_t t = a1905_mm((uint64_t)(a1905_C[2 * m][2 * i] % M),
                                      (uint64_t)(a1905_C[3 * m][3 * i] % M), M);
                t = a1905_mm(t, Vm[i], M);
                t = a1905_mm(t, Hm[m - i], M);
                sm = (sm + t) % M;
            }
            Vm[m] = (Hm[m] + M - sm) % M;
        }
        T = a1905_mm((uint64_t)(2 * N - 1) % M, Vm[N], M);
        v = 0;
        while (v < vQ && T % p == 0) { T /= p; v++; }
        for (e = v; e < vQ; e++) B *= (a1905_u128)p;
    }
    return a1905_from128(r * B);
}

static seq_t a1905_big(idx_t n) {
    seq_t A;
    uint64_t p, K;
    if (n >= A1905_LIM) return 0;
    while (a1905_cnt < n) {
        idx_t m = a1905_cnt + 1, i;
        seq_t s = 0, b2, b3;
        a1905_df = a1905_df * (seq_t)(2 * m - 1);
        a1905_fc = a1905_fc * (seq_t)(3 * m - 2) * (seq_t)(3 * m - 1) * (seq_t)(3 * m);
        a1905_H[m] = a1905_df * a1905_df * a1905_fc / (seq_t)(2 * m + 1);
        b2 = (seq_t)(2 * m) * (seq_t)(2 * m - 1) / 2;
        b3 = (seq_t)(3 * m) * (seq_t)(3 * m - 1) * (seq_t)(3 * m - 2) / 6;
        for (i = 1; i < m; i++) {
            s += b2 * b3 * a1905_V[i] * a1905_H[m - i];
            b2 = b2 * (seq_t)(2 * m - 2 * i) * (seq_t)(2 * m - 2 * i - 1)
                 / (seq_t)((2 * i + 1) * (2 * i + 2));
            b3 = b3 * (seq_t)(3 * m - 3 * i) * (seq_t)(3 * m - 3 * i - 1) * (seq_t)(3 * m - 3 * i - 2)
                 / (seq_t)((3 * i + 1) * (3 * i + 2) * (3 * i + 3));
        }
        a1905_V[m] = a1905_H[m] - s;
        a1905_cnt = m;
    }
    A = (seq_t)(2 * n - 1) * a1905_V[n];
    K = 3 * (uint64_t)n;
    for (p = 2; p <= K; p++) {
        uint64_t e;
        if (!a1905_isprime(p)) continue;
        e = a1905_leg(K, p);
        while (e > 0 && A % (seq_t)p == 0) { A /= (seq_t)p; e--; }
    }
    return A;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (a1905_isbig()) return a1905_big(n);
    return a1905_fixed(n);
}
