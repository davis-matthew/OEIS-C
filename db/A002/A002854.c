/*
 * A002854  Number of unlabeled Euler graphs with n nodes; number of unlabeled
 *          two-graphs with n nodes; number of unlabeled switching classes of
 *          graphs with n nodes; number of switching classes of unlabeled
 *          signed complete graphs on n nodes; number of Seidel matrices of
 *          order n.
 * https://oeis.org/A002854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = Sum_{s} 2^M(s)/Product_{i} i^s(i)*s(i)!, the sum over
 *            partitions s of n (s(i) parts equal to i), M(s) = Sum_{i<j}
 *            s(i)*s(j)*gcd(i,j) + Sum_{i} (s(i)*(floor(i/2) - 1) +
 *            i*binomial(s(i),2)) + sign(Sum_{k} s(2*k+1)). Partitions are
 *            enumerated with the parts equal to 1 handled in closed form....
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 12, u64 n <= 16, u128 n <= 21,
 *            big any n
 * Helpers:   q2854_addshift
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/q2854_addshift.h"

typedef unsigned __int128 q2854_u128;

static uint64_t *q2854_P = NULL, *q2854_PI = NULL, *q2854_R2 = NULL;
static int q2854_np = 0;

static int q2854_K;
static uint64_t *q2854_INV, *q2854_IF, *q2854_W, *q2854_bucket;
static long long *q2854_parts, *q2854_mults;
static unsigned short *q2854_gtab;
static long long q2854_maxe, q2854_n;

static uint64_t q2854_mulmod(uint64_t a, uint64_t b, uint64_t m) {
    return (uint64_t)((q2854_u128)a * b % m);
}

static uint64_t q2854_powmod(uint64_t b, uint64_t e, uint64_t m) {
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = q2854_mulmod(r, b, m);
        b = q2854_mulmod(b, b, m);
        e >>= 1;
    }
    return r;
}

static int q2854_isprime(uint64_t x) {
    static const uint64_t bases[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    if (x < 2) return 0;
    for (int i = 0; i < 12; i++) {
        if (x == bases[i]) return 1;
        if (x % bases[i] == 0) return 0;
    }
    uint64_t d = x - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (int i = 0; i < 12; i++) {
        uint64_t y = q2854_powmod(bases[i], d, x);
        if (y == 1 || y == x - 1) continue;
        int ok = 0;
        for (int r = 1; r < s; r++) {
            y = q2854_mulmod(y, y, x);
            if (y == x - 1) { ok = 1; break; }
        }
        if (!ok) return 0;
    }
    return 1;
}

static void q2854_ensure_primes(int K) {
    if (q2854_np >= K) return;
    q2854_P = (uint64_t *)realloc(q2854_P, (size_t)K * sizeof(uint64_t));
    q2854_PI = (uint64_t *)realloc(q2854_PI, (size_t)K * sizeof(uint64_t));
    q2854_R2 = (uint64_t *)realloc(q2854_R2, (size_t)K * sizeof(uint64_t));
    uint64_t cand = q2854_np ? q2854_P[q2854_np - 1] - 2 : (((uint64_t)1 << 62) - 1);
    while (q2854_np < K) {
        if (q2854_isprime(cand)) {
            uint64_t p = cand, inv = p;
            for (int t = 0; t < 6; t++) inv *= 2 - p * inv;
            q2854_P[q2854_np] = p;
            q2854_PI[q2854_np] = (uint64_t)0 - inv;
            uint64_t r = (uint64_t)(((q2854_u128)1 << 64) % p);
            q2854_R2[q2854_np] = q2854_mulmod(r, r, p);
            q2854_np++;
        }
        cand -= 2;
    }
}

static inline uint64_t q2854_mont(uint64_t a, uint64_t b, uint64_t p, uint64_t pinv) {
    q2854_u128 t = (q2854_u128)a * b;
    uint64_t m = (uint64_t)t * pinv;
    uint64_t r = (uint64_t)((t + (q2854_u128)m * p) >> 64);
    return r >= p ? r - p : r;
}

static void q2854_rec(long long rem, long long maxp, int depth, long long M, long long cnt, int odd) {
    int K = q2854_K;
    const uint64_t *W = q2854_W + (size_t)depth * K;
    long long s1 = rem;
    long long e = M - s1 + s1 * (s1 - 1) / 2 + s1 * cnt + ((odd || s1 > 0) ? 1 : 0);
    if (e >= 0 && e <= q2854_maxe) {
        uint64_t *b = q2854_bucket + (size_t)e * K;
        const uint64_t *f = q2854_IF + (size_t)s1 * K;
        for (int j = 0; j < K; j++) {
            uint64_t v = b[j] + q2854_mont(W[j], f[j], q2854_P[j], q2854_PI[j]);
            if (v >= q2854_P[j]) v -= q2854_P[j];
            b[j] = v;
        }
    }
    long long top = maxp < rem ? maxp : rem;
    uint64_t *Wc = q2854_W + (size_t)(depth + 1) * K;
    for (long long p = top; p >= 2; p--) {
        long long gsum = 0;
        const unsigned short *grow = q2854_gtab + (size_t)p * (size_t)(q2854_n + 1);
        for (int k = 0; k < depth; k++) gsum += q2854_mults[k] * (long long)grow[q2854_parts[k]];
        memcpy(Wc, W, (size_t)K * sizeof(uint64_t));
        q2854_parts[depth] = p;
        for (long long m = 1; m * p <= rem; m++) {
            const uint64_t *iv = q2854_INV + (size_t)(p * m) * K;
            for (int j = 0; j < K; j++) Wc[j] = q2854_mont(Wc[j], iv[j], q2854_P[j], q2854_PI[j]);
            long long Mn = M + m * (p / 2 - 1) + p * (m * (m - 1) / 2) + m * gsum;
            q2854_mults[depth] = m;
            q2854_rec(rem - m * p, p - 1, depth + 1, Mn, cnt + m, odd | (int)(p & 1));
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    long long N = (long long)n;
    q2854_n = N;
    uint64_t fbits = 1;
    for (long long i = 2; i <= N; i++) {
        long long t = i;
        while (t) { fbits++; t >>= 1; }
    }
    int K = (int)(fbits / 61 + 2);
    q2854_K = K;
    q2854_ensure_primes(K);

    q2854_INV = (uint64_t *)calloc((size_t)(N + 1) * K, sizeof(uint64_t));
    q2854_IF = (uint64_t *)calloc((size_t)(N + 1) * K, sizeof(uint64_t));
    int maxd = 2;
    while ((long long)maxd * (maxd + 3) / 2 <= N) maxd++;
    maxd += 3;
    q2854_W = (uint64_t *)calloc((size_t)(maxd + 1) * K, sizeof(uint64_t));
    q2854_parts = (long long *)calloc((size_t)maxd, sizeof(long long));
    q2854_mults = (long long *)calloc((size_t)maxd, sizeof(long long));
    q2854_gtab = (unsigned short *)malloc((size_t)(N + 1) * (size_t)(N + 1) * sizeof(unsigned short));
    for (long long x = 0; x <= N; x++)
        for (long long y = 0; y <= N; y++) {
            long long u = x, v = y;
            while (v) { long long t = u % v; u = v; v = t; }
            q2854_gtab[(size_t)x * (size_t)(N + 1) + (size_t)y] = (unsigned short)u;
        }

    for (int j = 0; j < K; j++) {
        uint64_t p = q2854_P[j], pi = q2854_PI[j], r2 = q2854_R2[j];
        uint64_t fact = 1;
        q2854_IF[j] = q2854_mont(1, r2, p, pi);
        for (long long x = 1; x <= N; x++) {
            uint64_t iv = q2854_powmod((uint64_t)x, p - 2, p);
            q2854_INV[(size_t)x * K + j] = q2854_mont(iv, r2, p, pi);
            q2854_IF[(size_t)x * K + j] = q2854_mont(q2854_IF[(size_t)(x - 1) * K + j],
                                                     q2854_INV[(size_t)x * K + j], p, pi);
            fact = q2854_mulmod(fact, (uint64_t)x, p);
        }
        q2854_W[j] = q2854_mont(fact, r2, p, pi);
    }

    q2854_maxe = N * (N - 1) / 2 + 1;
    q2854_bucket = (uint64_t *)calloc((size_t)(q2854_maxe + 1) * K, sizeof(uint64_t));

    q2854_rec(N, N, 0, 0, 0, 0);

    uint64_t totbits = (uint64_t)q2854_maxe + fbits + 64;
    size_t AL = (size_t)(totbits / 32 + 4);
    uint32_t *acc = (uint32_t *)calloc(AL, sizeof(uint32_t));
    size_t BL = (size_t)(2 * K + 2);
    uint32_t *tmp = (uint32_t *)malloc(BL * sizeof(uint32_t));
    uint64_t *res = (uint64_t *)malloc((size_t)K * sizeof(uint64_t));
    uint64_t *dig = (uint64_t *)malloc((size_t)K * sizeof(uint64_t));
    uint64_t *invtab = (uint64_t *)malloc((size_t)K * K * sizeof(uint64_t));
    for (int i = 0; i < K; i++)
        for (int j = 0; j < K; j++)
            invtab[i * K + j] = (i == j) ? 0 : q2854_powmod(q2854_P[i] % q2854_P[j], q2854_P[j] - 2, q2854_P[j]);

    for (long long e = 0; e <= q2854_maxe; e++) {
        const uint64_t *b = q2854_bucket + (size_t)e * K;
        int nz = 0;
        for (int j = 0; j < K; j++) {
            res[j] = q2854_mont(b[j], 1, q2854_P[j], q2854_PI[j]);
            if (res[j]) nz = 1;
        }
        if (!nz) continue;
        for (int j = 0; j < K; j++) {
            uint64_t p = q2854_P[j];
            uint64_t v = res[j];
            for (int i = 0; i < j; i++) {
                uint64_t ci = dig[i] % p;
                v = v >= ci ? v - ci : v + p - ci;
                v = q2854_mulmod(v, invtab[i * K + j], p);
            }
            dig[j] = v;
        }
        memset(tmp, 0, BL * sizeof(uint32_t));
        tmp[0] = (uint32_t)dig[K - 1];
        tmp[1] = (uint32_t)(dig[K - 1] >> 32);
        for (int i = K - 2; i >= 0; i--) {
            q2854_u128 carry = 0;
            for (size_t l = 0; l < BL; l++) {
                q2854_u128 t = (q2854_u128)tmp[l] * q2854_P[i] + carry;
                tmp[l] = (uint32_t)t;
                carry = t >> 32;
            }
            uint64_t c = dig[i];
            for (size_t l = 0; l < BL && c; l++) {
                uint64_t s = (uint64_t)tmp[l] + (c & 0xffffffffu);
                tmp[l] = (uint32_t)s;
                c = (c >> 32) + (s >> 32);
            }
        }
        q2854_addshift(acc, AL, tmp, BL, (uint64_t)e);
    }

    for (long long i = 2; i <= N; i++) {
        uint64_t r = 0;
        for (size_t k = AL; k-- > 0;) {
            uint64_t cur = (r << 32) | acc[k];
            acc[k] = (uint32_t)(cur / (uint64_t)i);
            r = cur % (uint64_t)i;
        }
    }

    size_t top = AL;
    while (top > 0 && acc[top - 1] == 0) top--;
    seq_t result = 0;
    for (size_t k = top; k-- > 0;) {
        result = result * 65536;
        result = result * 65536;
        result += (seq_t)acc[k];
    }

    free(acc); free(tmp); free(res); free(dig); free(invtab);
    free(q2854_INV); free(q2854_IF); free(q2854_W);
    free(q2854_parts); free(q2854_mults); free(q2854_gtab); free(q2854_bucket);
    return result;
}
