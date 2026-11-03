/*
 * A000851  Number of n-input 3-output switching networks under action of
 *          AG(n,2) on the inputs and complementing group C(3,2) on the
 *          outputs.
 * https://oeis.org/A000851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside's lemma over G = AG(n,2) x C(3,2): a(n) =
 *            (1/(8*2^n*|GL(n,2)|)) * Sum_{g in AG(n,2)} 8^cyc(g) * (1 +
 *            7*[all cycles of g on F_2^n have even length]), where the sum
 *            over g = (x -> A*x + b) runs over conjugacy classes of GL(n,2)
 *            (rational canonical forms, class size |GL(n,2)|/|C(A)|) and
 *            ov...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-01)
 * Helpers:   ilog2_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ilog2_u64.h"

static void z851_addbig(uint32_t *x, const uint32_t *y, int L) {
    uint64_t c = 0;
    for (int i = 0; i < L; i++) {
        uint64_t s = (uint64_t)x[i] + y[i] + c;
        x[i] = (uint32_t)s;
        c = s >> 32;
    }
}

static void z851_addsmall(uint32_t *x, int L, uint64_t v) {
    for (int i = 0; i < L && v; i++) {
        uint64_t s = (uint64_t)x[i] + (v & 0xffffffffull);
        x[i] = (uint32_t)s;
        v = (v >> 32) + (s >> 32);
    }
}

static void z851_div(uint32_t *x, int L, uint32_t d) {
    uint64_t r = 0;
    for (int i = L - 1; i >= 0; i--) {
        uint64_t v = (r << 32) | x[i];
        x[i] = (uint32_t)(v / d);
        r = v % d;
    }
}

static void z851_mul(uint32_t *x, int L, uint32_t m) {
    uint64_t c = 0;
    for (int i = 0; i < L; i++) {
        uint64_t v = (uint64_t)x[i] * m + c;
        x[i] = (uint32_t)v;
        c = v >> 32;
    }
}

static uint64_t z851_pmod(uint64_t f, uint64_t g) {
    int dg = ilog2_u64(g);
    int df = ilog2_u64(f);
    while (df >= dg) {
        f ^= g << (df - dg);
        df = ilog2_u64(f);
    }
    return f;
}

static uint64_t z851_pmul(uint64_t a, uint64_t b) {
    uint64_t r = 0;
    while (b) {
        if (b & 1) r ^= a;
        a <<= 1;
        b >>= 1;
    }
    return r;
}

static void z851_shl(uint32_t *x, int L, long long s) {
    long long ws = s / 32;
    int bs = (int)(s % 32);
    for (long long i = L - 1; i >= 0; i--) {
        uint64_t v = 0;
        long long j = i - ws;
        if (j >= 0) {
            v = (uint64_t)x[j] << bs;
            if (bs && j - 1 >= 0) v |= (uint64_t)x[j - 1] >> (32 - bs);
        }
        x[i] = (uint32_t)v;
    }
}

static void z851_shr(uint32_t *x, int L, long long s) {
    long long ws = s / 32;
    int bs = (int)(s % 32);
    for (long long i = 0; i < L; i++) {
        uint64_t v = 0;
        long long j = i + ws;
        if (j < L) {
            v = (uint64_t)x[j] >> bs;
            if (bs && j + 1 < L) v |= (uint64_t)x[j + 1] << (32 - bs);
        }
        x[i] = (uint32_t)v;
    }
}

static int zN, zL, znirr, zncls;
static uint64_t *zirr;
static int *zirrd;
static int zcp[64], zce[64];
static uint32_t *zT, *zP, *zTot;
static unsigned char *zvis;
static uint64_t *zh;

static void z851_process(void) {
    int n = zN, L = zL;
    uint32_t col[32];
    int pos = 0;
    for (int t = 0; t < zncls; t++) {
        uint64_t f = 1;
        for (int k = 0; k < zce[t]; k++) f = z851_pmul(f, zirr[zcp[t]]);
        int D = zirrd[zcp[t]] * zce[t];
        for (int i = 0; i < D - 1; i++) col[pos + i] = 1u << (pos + i + 1);
        col[pos + D - 1] = (uint32_t)(f & ((1ull << D) - 1)) << pos;
        pos += D;
    }
    uint64_t N = 1ull << n;
    zT[0] = 0;
    for (uint64_t x = 1; x < N; x++) {
        int idx = 0;
        while (!((x >> idx) & 1)) idx++;
        zT[x] = zT[x & (x - 1)] ^ col[idx];
    }
    /* image of A - I */
    uint32_t basis[32];
    uint32_t piv = 0;
    int rank = 0;
    for (int i = 0; i < n; i++) basis[i] = 0;
    for (int i = 0; i < n; i++) {
        uint32_t v = col[i] ^ (1u << i);
        for (int b = n - 1; b >= 0; b--) {
            if (!((v >> b) & 1)) continue;
            if (basis[b]) v ^= basis[b];
            else { basis[b] = v; piv |= 1u << b; rank++; break; }
        }
    }
    memset(zh, 0, sizeof(uint64_t) * (size_t)(N + 1));
    for (uint64_t b = 0; b < N; b++) {
        if ((uint32_t)b & piv) continue;
        memset(zvis, 0, (size_t)N);
        uint64_t cyc = 0;
        int odd = 0;
        for (uint64_t x = 0; x < N; x++) {
            if (zvis[x]) continue;
            uint64_t len = 0, y = x;
            do {
                zvis[y] = 1;
                y = zT[y] ^ (uint32_t)b;
                len++;
            } while (y != x);
            cyc++;
            if (len & 1) odd = 1;
        }
        zh[cyc] += odd ? 1 : 8;
    }
    memset(zP, 0, sizeof(uint32_t) * (size_t)L);
    for (long long c = (long long)N; c >= 0; c--) {
        z851_mul(zP, L, 8);
        z851_addsmall(zP, L, zh[c]);
    }
    /* multiply by |GL(n,2)| */
    z851_shl(zP, L, (long long)rank + (long long)n * (n - 1) / 2);
    for (int k = 1; k <= n; k++) z851_mul(zP, L, (uint32_t)((1ull << k) - 1));
    /* divide by centralizer order */
    long long E = 0;
    int t = 0;
    while (t < zncls) {
        int p = zcp[t];
        int s = t;
        while (t < zncls && zcp[t] == p) t++;
        int d = zirrd[p];
        int e1 = zce[s];
        long long sq = 0;
        for (int j = 1; j <= e1; j++) {
            long long c = 0;
            for (int u = s; u < t; u++) if (zce[u] >= j) c++;
            sq += c * c;
        }
        E += (long long)d * sq;
        int u = s;
        while (u < t) {
            int v = u;
            while (v < t && zce[v] == zce[u]) v++;
            long long m = v - u;
            E -= (long long)d * m * (m + 1) / 2;
            for (long long k = 1; k <= m; k++)
                z851_div(zP, L, (uint32_t)((1ull << (d * k)) - 1));
            u = v;
        }
    }
    z851_shr(zP, L, E);
    z851_addbig(zTot, zP, L);
}

static void z851_rec(int pi, int rem, int maxp) {
    if (rem == 0) { z851_process(); return; }
    if (pi >= znirr || zirrd[pi] > rem) return;
    z851_rec(pi + 1, rem, 1000);
    int d = zirrd[pi];
    for (int e = 1; e <= maxp && d * e <= rem; e++) {
        zcp[zncls] = pi;
        zce[zncls] = e;
        zncls++;
        z851_rec(pi, rem - d * e, e);
        zncls--;
    }
}

seq_t a(idx_t n) {
    int nn = (int)n;
    zN = nn;
    /* irreducible polynomials over F_2 of degree 1..n, excluding x */
    zirr = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)2 << nn));
    zirrd = (int *)malloc(sizeof(int) * ((size_t)2 << nn));
    znirr = 0;
    for (int d = 1; d <= nn; d++) {
        for (uint64_t f = (1ull << d) | 1; f < (2ull << d); f += 2) {
            int ok = 1;
            for (uint64_t g = 2; ok && ilog2_u64(g) <= d / 2; g++)
                if (z851_pmod(f, g) == 0) ok = 0;
            if (ok) { zirr[znirr] = f; zirrd[znirr] = d; znirr++; }
        }
    }
    uint64_t N = 1ull << nn;
    long long bits = 3 * (long long)N + (long long)nn * nn + 4LL * nn + 96;
    zL = (int)(bits / 32 + 2);
    zT = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)N);
    zvis = (unsigned char *)malloc((size_t)N);
    zh = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)(N + 1));
    zP = (uint32_t *)calloc((size_t)zL, sizeof(uint32_t));
    zTot = (uint32_t *)calloc((size_t)zL, sizeof(uint32_t));
    zncls = 0;
    z851_rec(0, nn, 1000);
    z851_shr(zTot, zL, 3 + (long long)nn + (long long)nn * (nn - 1) / 2);
    for (int k = 1; k <= nn; k++) z851_div(zTot, zL, (uint32_t)((1ull << k) - 1));
    seq_t r = 0;
    for (int i = zL - 1; i >= 0; i--) {
        for (int sh = 24; sh >= 0; sh -= 8) {
            r = r * 256 + (seq_t)(unsigned)((zTot[i] >> sh) & 255u);
        }
    }
    free(zirr); free(zirrd); free(zT); free(zvis); free(zh); free(zP); free(zTot);
    return r;
}
