/*
 * A002298  Denominator of (2/Pi)*Integral_{0..inf} (sin x / x)^n dx.
 * https://oeis.org/A002298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy, nice
 * Formula:   (2/Pi)*Integral_{0..inf} (sin x/x)^n dx = S(n)/(2^(n-1)*(n-1)!),
 *            S(n) = Sum_{k=0..floor(n/2)} (-1)^k*binomial(n,k)*(n-2k)^(n-1);
 *            a(n) = 2^(n-1)*(n-1)! / gcd(S(n), 2^(n-1)*(n-1)!). For even n =
 *            2m this equals (n-1)!/gcd(T, (n-1)!) with T = Sum_{k=0..m}
 *            (-1)^k*binomial(n,k)*(m-k)^(n-1). The gcd is...
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 30,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 96 (2026-11-01)
 * Helpers:   modinv, mulmod, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/modinv.h"
#include "../utils/mulmod.h"
#include "../utils/powmod_u64.h"

static void q_badd(uint32_t *x, const uint32_t *y, size_t L) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t t = (uint64_t)x[i] + y[i] + c;
        x[i] = (uint32_t)t;
        c = t >> 32;
    }
}

static uint32_t q_bdiv(uint32_t *x, size_t L, uint32_t d) {
    uint64_t r = 0;
    size_t i = L;
    while (i--) {
        uint64_t cur = (r << 32) | x[i];
        x[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
    return (uint32_t)r;
}

static void q_bmul(uint32_t *x, size_t L, uint64_t m) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t t = (uint64_t)x[i] * m + c;
        x[i] = (uint32_t)t;
        c = t >> 32;
    }
}

static void q_bsub(uint32_t *x, const uint32_t *y, size_t L) {
    uint64_t br = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t t = (uint64_t)x[i] - y[i] - br;
        x[i] = (uint32_t)t;
        br = (t >> 63) & 1;
    }
}

/* exact S (or T for even n) as a little-endian limb array */
static uint32_t *q_exact(idx_t n, size_t *len) {
    int odd = (int)(n & 1);
    idx_t cnt = odd ? (n - 1) / 2 + 1 : n / 2;
    int bl = 0;
    idx_t x = n, k, i;
    size_t L;
    uint32_t *pos, *neg, *C, *T;
    while (x) { bl++; x >>= 1; }
    L = (size_t)((n + (n - 1) * (idx_t)bl + 64) / 32 + 2);
    pos = (uint32_t *)calloc(L, sizeof(uint32_t));
    neg = (uint32_t *)calloc(L, sizeof(uint32_t));
    C = (uint32_t *)calloc(L, sizeof(uint32_t));
    T = (uint32_t *)calloc(L, sizeof(uint32_t));
    C[0] = 1;
    for (k = 0; k < cnt; k++) {
        idx_t bb = odd ? n - 2 * k : n / 2 - k;
        if (k > 0) {
            q_bmul(C, L, n - k + 1);
            q_bdiv(C, L, (uint32_t)k);
        }
        memcpy(T, C, L * sizeof(uint32_t));
        for (i = 0; i + 1 < n; i++) q_bmul(T, L, bb);
        if (k & 1) q_badd(neg, T, L);
        else q_badd(pos, T, L);
    }
    q_bsub(pos, neg, L);
    free(neg); free(C); free(T);
    *len = L;
    return pos;
}

/* S (or T for even n) modulo M = p^K */
static uint64_t q_res(idx_t n, uint64_t p, int K, uint64_t M,
                      const uint32_t *spf, uint64_t *f, uint64_t *iu) {
    int odd = (int)(n & 1);
    idx_t cnt = odd ? (n - 1) / 2 + 1 : n / 2;
    idx_t bmax = odd ? n : n / 2;
    uint64_t pp[66];
    idx_t b, k;
    uint64_t u = 1 % M, S = 0, inv;
    int v = 0, i;

    pp[0] = 1 % M;
    for (i = 1; i < K; i++) pp[i] = mulmod(pp[i - 1], p, M);

    f[1] = 1 % M;
    for (b = 2; b <= bmax; b++) {
        uint32_t q = spf[b];
        if ((idx_t)q == b) f[b] = powmod_u64(b, n - 1, M);
        else f[b] = mulmod(f[q], f[b / q], M);
    }

    iu[0] = 1 % M;
    for (k = 1; k < cnt; k++) {
        uint64_t y = k;
        while (y % p == 0) y /= p;
        iu[k] = mulmod(iu[k - 1], y % M, M);
    }
    if (cnt > 1) {
        inv = modinv(iu[cnt - 1], M);
        for (k = cnt - 1; k >= 1; k--) {
            uint64_t y = k;
            while (y % p == 0) y /= p;
            iu[k] = mulmod(inv, iu[k - 1], M);
            inv = mulmod(inv, y % M, M);
        }
    }

    for (k = 0; k < cnt; k++) {
        if (k > 0) {
            uint64_t x = n - k + 1, y = k;
            while (x % p == 0) { x /= p; v++; }
            u = mulmod(u, x % M, M);
            while (y % p == 0) { y /= p; v--; }
            u = mulmod(u, iu[k], M);
        }
        if (v < K) {
            idx_t bb = odd ? n - 2 * k : n / 2 - k;
            uint64_t t = mulmod(mulmod(u, pp[v], M), f[bb], M);
            if (k & 1) S = (S + M - t) % M;
            else S = (S + t) % M;
        }
    }
    return S;
}

seq_t a(idx_t n) {
    int odd = (int)(n & 1);
    uint32_t *spf, *ex = NULL, *tmp = NULL;
    uint64_t *f, *iu;
    size_t exL = 0;
    idx_t i, j, p;
    seq_t r = 1;

    spf = (uint32_t *)calloc((size_t)n + 2, sizeof(uint32_t));
    f = (uint64_t *)calloc((size_t)n + 2, sizeof(uint64_t));
    iu = (uint64_t *)calloc((size_t)n + 2, sizeof(uint64_t));
    for (i = 2; i <= n; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= n; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }

    for (p = 2; p + 1 <= n; p++) {
        idx_t e = 0, x = n - 1, vS = 0, t;
        int done = 0, lvl;
        if (spf[p] != (uint32_t)p) continue;
        while (x) { x /= p; e += x; }
        if (p == 2 && odd) e += n - 1;
        if (e == 0) continue;

        for (lvl = 0; lvl < 2 && !done; lvl++) {
            uint64_t lim = lvl == 0 ? 0xFFFFFFFFULL : (1ULL << 62);
            uint64_t M = 1, S;
            int K = 0;
            while ((idx_t)K < e && M <= lim / p) { M *= p; K++; }
            S = q_res(n, p, K, M, spf, f, iu);
            if (S != 0) {
                while (S % p == 0) { S /= p; vS++; }
                done = 1;
            } else if ((idx_t)K == e) {
                vS = e;
                done = 1;
            }
        }
        if (!done) {
            if (!ex) {
                ex = q_exact(n, &exL);
                tmp = (uint32_t *)malloc(exL * sizeof(uint32_t));
            }
            {
                uint32_t *w = (uint32_t *)malloc(exL * sizeof(uint32_t));
                memcpy(w, ex, exL * sizeof(uint32_t));
                while (vS < e) {
                    memcpy(tmp, w, exL * sizeof(uint32_t));
                    if (q_bdiv(tmp, exL, (uint32_t)p) != 0) break;
                    memcpy(w, tmp, exL * sizeof(uint32_t));
                    vS++;
                }
                free(w);
            }
        }
        for (t = vS; t < e; t++) r *= (seq_t)p;
    }

    free(spf); free(f); free(iu);
    if (ex) { free(ex); free(tmp); }
    return r;
}
