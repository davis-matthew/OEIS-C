/*
 * A004673  Theta series of extremal even unimodular lattice in dimension 56.
 * https://oeis.org/A004673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [q^n] (E4^7 - 1680*E4^4*Delta + 347760*E4*Delta^2), where
 *            E4 = 1 + 240*Sum_{k>=1} sigma_3(k) q^k, E6 = 1 - 504*Sum_{k>=1}
 *            sigma_5(k) q^k, Delta = (E4^3 - E6^2)/1728. Series are evaluated
 *            by NTT modulo several primes, and a(n) is rebuilt by CRT
 *            (Garner)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 8, u128 n <= 43,
 *            big any n
 * Helpers:   isprime32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/isprime32.h"

#define NTT_SHIFT 22
#define MAXP 64

static uint32_t PR[MAXP], GR[MAXP];
static int npr = 0;

static uint64_t pw_mod(uint64_t b, uint64_t e, uint64_t m) {
    uint64_t r = 1;
    b %= m;
    while (e) {
        if (e & 1) r = r * b % m;
        b = b * b % m;
        e >>= 1;
    }
    return r;
}

static void init_primes(void) {
    if (npr) return;
    for (uint32_t c = (1u << (31 - NTT_SHIFT)) - 1; c >= 1 && npr < MAXP; c--) {
        uint64_t p = ((uint64_t)c << NTT_SHIFT) + 1;
        if (p >= (1ull << 31) || !isprime32(p)) continue;
        uint64_t fac[16];
        int nf = 0;
        fac[nf++] = 2;
        uint64_t cc = c;
        for (uint64_t q = 2; q * q <= cc; q++) {
            if (cc % q == 0) {
                if (q != 2) fac[nf++] = q;
                while (cc % q == 0) cc /= q;
            }
        }
        if (cc > 1 && cc != 2) fac[nf++] = cc;
        uint64_t g = 2;
        for (;; g++) {
            bool ok = true;
            for (int i = 0; i < nf; i++)
                if (pw_mod(g, (p - 1) / fac[i], p) == 1) { ok = false; break; }
            if (ok) break;
        }
        PR[npr] = (uint32_t)p;
        GR[npr] = (uint32_t)g;
        npr++;
    }
}

static void ntt(uint32_t *a, size_t L, int inv, uint64_t p, uint64_t g) {
    for (size_t i = 1, j = 0; i < L; i++) {
        size_t bit = L >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) { uint32_t t = a[i]; a[i] = a[j]; a[j] = t; }
    }
    for (size_t len = 2; len <= L; len <<= 1) {
        uint64_t w = pw_mod(g, (p - 1) / len, p);
        if (inv) w = pw_mod(w, p - 2, p);
        size_t half = len >> 1;
        for (size_t i = 0; i < L; i += len) {
            uint64_t wn = 1;
            for (size_t k = 0; k < half; k++) {
                uint64_t u = a[i + k];
                uint64_t v = (uint64_t)a[i + k + half] * wn % p;
                a[i + k] = (uint32_t)((u + v) % p);
                a[i + k + half] = (uint32_t)((u + p - v) % p);
                wn = wn * w % p;
            }
        }
    }
    if (inv) {
        uint64_t il = pw_mod(L, p - 2, p);
        for (size_t i = 0; i < L; i++) a[i] = (uint32_t)((uint64_t)a[i] * il % p);
    }
}

static uint32_t *cache_res = NULL;
static idx_t cache_cnt = 0;
static int cache_K = 0;

static void compute(idx_t N) {
    init_primes();
    int lg = 0;
    for (uint64_t t = (uint64_t)N + 2; t; t >>= 1) lg++;
    int need = 30 * lg + 80;
    int K = 0, have = 0;
    while (K < npr && have < need) {
        int b = 0;
        for (uint32_t t = PR[K]; t > 1; t >>= 1) b++;
        have += b;
        K++;
    }
    size_t L = 1;
    while (L < (size_t)(7 * (uint64_t)N + 1)) L <<= 1;
    idx_t cnt = N + 1;
    uint32_t *res = (uint32_t *)malloc((size_t)K * cnt * sizeof(uint32_t));
    uint32_t *A = (uint32_t *)malloc(L * sizeof(uint32_t));
    uint32_t *B = (uint32_t *)malloc(L * sizeof(uint32_t));
    for (int k = 0; k < K; k++) {
        uint64_t p = PR[k], g = GR[k];
        memset(A, 0, L * sizeof(uint32_t));
        memset(B, 0, L * sizeof(uint32_t));
        for (uint64_t d = 1; d <= N; d++) {
            uint64_t dm = d % p;
            uint64_t d2 = dm * dm % p;
            uint64_t d3 = d2 * dm % p;
            uint64_t d5 = d3 * d2 % p;
            for (uint64_t m = d; m <= N; m += d) {
                A[m] = (uint32_t)((A[m] + d3) % p);
                B[m] = (uint32_t)((B[m] + d5) % p);
            }
        }
        A[0] = 1;
        B[0] = 1;
        for (uint64_t m = 1; m <= N; m++) {
            A[m] = (uint32_t)(240ull * A[m] % p);
            B[m] = (uint32_t)((p - 504ull * B[m] % p) % p);
        }
        ntt(A, L, 0, p, g);
        ntt(B, L, 0, p, g);
        uint64_t inv1728 = pw_mod(1728, p - 2, p);
        for (size_t i = 0; i < L; i++) {
            uint64_t e = A[i], f = B[i];
            uint64_t e2 = e * e % p, e3 = e2 * e % p, e4 = e2 * e2 % p;
            uint64_t e7 = e4 * e3 % p;
            uint64_t d = (e3 + p - f * f % p) % p * inv1728 % p;
            uint64_t t1 = 1680ull * e4 % p * d % p;
            uint64_t t2 = 347760ull * e % p * d % p * d % p;
            A[i] = (uint32_t)((e7 + p - t1 + t2) % p);
        }
        ntt(A, L, 1, p, g);
        for (idx_t m = 0; m < cnt; m++) res[(size_t)k * cnt + m] = A[m];
    }
    free(A);
    free(B);
    free(cache_res);
    cache_res = res;
    cache_cnt = cnt;
    cache_K = K;
}

seq_t a(idx_t n) {
    if (n + 1 > cache_cnt) {
        idx_t N = n;
        if (N < 2 * cache_cnt) N = 2 * cache_cnt;
        if (N < 256) N = 256;
        compute(N);
    }
    int K = cache_K;
    uint64_t dg[MAXP];
    for (int i = 0; i < K; i++) {
        uint64_t p = PR[i];
        uint64_t v = cache_res[(size_t)i * cache_cnt + n];
        for (int j = 0; j < i; j++) {
            uint64_t inv = pw_mod(PR[j] % p, p - 2, p);
            v = (v + p - dg[j] % p) % p * inv % p;
        }
        dg[i] = v;
    }
    seq_t r = (seq_t)dg[K - 1];
    for (int i = K - 2; i >= 0; i--)
        r = r * (seq_t)PR[i] + (seq_t)dg[i];
    return r;
}
