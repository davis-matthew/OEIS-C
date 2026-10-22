/*
 * A000639  Number of alkyl benzenes with n carbon atoms: C(n)H(2n-6).
 * https://oeis.org/A000639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = [x^(n-6)] (B(x)^6 + 4*B(x^2)^3 + 2*B(x^3)^2 +
 *            3*B(x)^2*B(x^2)^2 + 2*B(x^6))/12, where B(x) = 1 + x*(B(x)^3 +
 *            3*B(x)*B(x^2) + 2*B(x^3))/6 is the g.f. of A000598
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 29, u64 n <= 52, u128 n <= 95,
 *            big any n
 * Helpers:   a639_grow, a639_growp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a639_grow.h"
#include "../utils/a639_growp.h"

#define A639_P 1000000007ULL

static int a639_fx = -1;
static int a639_W = 0;
static seq_t a639_inv3;
static uint64_t a639_inv2w, a639_inv6p, a639_inv12p;

static uint64_t a639_pw(uint64_t b, uint64_t e) {
    uint64_t r = 1;
    b %= A639_P;
    while (e) {
        if (e & 1) r = r * b % A639_P;
        b = b * b % A639_P;
        e >>= 1;
    }
    return r;
}

static void a639_init(void) {
    seq_t t = 1;
    int i;
    for (i = 0; i < 300 && t != 0; i++) t <<= 1;
    a639_fx = (t == 0) ? 1 : 0;
    a639_W = i;
    if (a639_fx) {
        seq_t x = 3;
        uint64_t tw = 1;
        for (i = 0; i < 8; i++) x = (seq_t)(x * (seq_t)((seq_t)2 - (seq_t)3 * x));
        a639_inv3 = x;
        for (i = 0; i < a639_W; i++) tw = tw * 2 % A639_P;
        a639_inv2w = a639_pw(tw, A639_P - 2);
        a639_inv6p = a639_pw(6, A639_P - 2);
        a639_inv12p = a639_pw(12, A639_P - 2);
    }
}

/* exact division of N by 3*2^s; L = N mod 2^W (or N exactly), R = N mod P */
static seq_t a639_exdiv(seq_t L, uint64_t R, int s) {
    uint64_t l, q;
    seq_t h;
    if (!a639_fx) return L / (3 << s);
    if (a639_W <= 64) l = (uint64_t)L % A639_P;
    else l = (uint64_t)(L % (seq_t)A639_P);
    q = (R + A639_P - l) % A639_P * a639_inv2w % A639_P;
    h = (seq_t)(L >> s);
    h += (seq_t)((seq_t)q << (a639_W - s));
    return (seq_t)(h * a639_inv3);
}

static seq_t *Bv = NULL, *S2v = NULL, *S3v = NULL;
static uint64_t *Bp = NULL, *S2p = NULL, *S3p = NULL;
static idx_t a639_have = 0, a639_cap = 0;

static void a639_ensure(idx_t K) {
    idx_t m, i;
    if (K < a639_have) return;
    if (K + 1 > a639_cap) {
        idx_t nc = a639_cap ? a639_cap : 64;
        while (nc < K + 1) nc *= 2;
        Bv = a639_grow(Bv, a639_have, nc);
        S2v = a639_grow(S2v, a639_have, nc);
        S3v = a639_grow(S3v, a639_have, nc);
        Bp = a639_growp(Bp, a639_have, nc);
        S2p = a639_growp(S2p, a639_have, nc);
        S3p = a639_growp(S3p, a639_have, nc);
        a639_cap = nc;
    }
    for (m = a639_have; m <= K; m++) {
        if (m == 0) {
            Bv[0] = 1; S2v[0] = 1; S3v[0] = 1;
            Bp[0] = 1; S2p[0] = 1; S3p[0] = 1;
            continue;
        }
        {
            idx_t j = m - 1;
            seq_t N = S3v[j];
            seq_t t = 0;
            uint64_t Np = S3p[j], tp = 0;
            for (i = 0; 2 * i <= j; i++) {
                t += Bv[i] * Bv[j - 2 * i];
                if (a639_fx) tp = (tp + Bp[i] * Bp[j - 2 * i]) % A639_P;
            }
            N += (seq_t)3 * t;
            Np = (Np + 3 * tp) % A639_P;
            if (j % 3 == 0) {
                N += (seq_t)2 * Bv[j / 3];
                Np = (Np + 2 * Bp[j / 3]) % A639_P;
            }
            Bv[m] = a639_exdiv(N, Np, 1);
            if (a639_fx) Bp[m] = Np * a639_inv6p % A639_P;
        }
        {
            seq_t s2 = 0;
            uint64_t s2p = 0;
            for (i = 0; i <= m; i++) {
                s2 += Bv[i] * Bv[m - i];
                if (a639_fx) s2p = (s2p + Bp[i] * Bp[m - i]) % A639_P;
            }
            S2v[m] = s2;
            S2p[m] = s2p;
        }
        {
            seq_t s3 = 0;
            uint64_t s3p = 0;
            for (i = 0; i <= m; i++) {
                s3 += Bv[i] * S2v[m - i];
                if (a639_fx) s3p = (s3p + Bp[i] * S2p[m - i]) % A639_P;
            }
            S3v[m] = s3;
            S3p[m] = s3p;
        }
    }
    a639_have = K + 1;
}

seq_t a(idx_t n) {
    idx_t K, i;
    seq_t N = 0, t = 0;
    uint64_t Np = 0, tp = 0;
    if (n < 6) return 0;
    if (a639_fx < 0) a639_init();
    K = n - 6;
    a639_ensure(K);
    for (i = 0; i <= K; i++) {
        N += S3v[i] * S3v[K - i];
        if (a639_fx) Np = (Np + S3p[i] * S3p[K - i]) % A639_P;
    }
    for (i = 0; 2 * i <= K; i++) {
        t += S2v[i] * S2v[K - 2 * i];
        if (a639_fx) tp = (tp + S2p[i] * S2p[K - 2 * i]) % A639_P;
    }
    N += (seq_t)3 * t;
    Np = (Np + 3 * tp) % A639_P;
    if (K % 2 == 0) {
        N += (seq_t)4 * S3v[K / 2];
        Np = (Np + 4 * S3p[K / 2]) % A639_P;
    }
    if (K % 3 == 0) {
        N += (seq_t)2 * S2v[K / 3];
        Np = (Np + 2 * S2p[K / 3]) % A639_P;
    }
    if (K % 6 == 0) {
        N += (seq_t)2 * Bv[K / 6];
        Np = (Np + 2 * Bp[K / 6]) % A639_P;
    }
    (void)a639_inv12p;
    return a639_exdiv(N, Np, 2);
}
