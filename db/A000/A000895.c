/*
 * A000895  Number of switching networks with AG(n,2) acting on the domain and
 *          AG(2,2) acting on the range.
 * https://oeis.org/A000895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside's lemma. a(n) = (1/(24*|AGL(n,2)|)) * Sum_{g in
 *            AGL(n,2)} Sum_{h in S_4 = AGL(2,2)} Prod_{cycles c of g}
 *            #Fix(h^{|c|}). The conjugacy classes of AGL(n,2) come from the
 *            primary decomposition of GF(2)^n over GF(2)[x]. Only the
 *            unipotent block keeps a translation part
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   z_deg, z_gcd, z_next_part, z_pmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/z_deg.h"
#include "../utils/z_gcd.h"
#include "../utils/z_next_part.h"
#include "../utils/z_pmod.h"

static int z_n, z_T, z_L, z_ncomp, z_ulen, z_m;
static int *z_pd; static uint64_t *z_pe;
static int *z_cd; static uint64_t *z_ce; static int **z_cpart; static int *z_clen;
static int *z_ulam;
static uint32_t *z_glodd, *z_G, *z_Y, *z_tot;

static void z_shl_add(uint32_t *x, uint64_t e, uint64_t m) {
    size_t i = (size_t)(e >> 5);
    int b = (int)(e & 31);
    uint64_t v = m << b;
    while (v && i < (size_t)z_L) {
        uint64_t s = (uint64_t)x[i] + (v & 0xffffffffu);
        x[i] = (uint32_t)s;
        v = (v >> 32) + (s >> 32);
        i++;
    }
}
static void z_mul(uint32_t *x, uint32_t m) {
    uint64_t c = 0;
    for (int i = 0; i < z_L; i++) { uint64_t t = (uint64_t)x[i] * m + c; x[i] = (uint32_t)t; c = t >> 32; }
}
static void z_div(uint32_t *x, uint32_t d) {
    uint64_t r = 0;
    for (int i = z_L - 1; i >= 0; i--) { uint64_t t = (r << 32) | x[i]; x[i] = (uint32_t)(t / d); r = t % d; }
}
static void z_shr(uint32_t *x, uint64_t s) {
    size_t q = (size_t)(s >> 5); int b = (int)(s & 31);
    for (size_t i = 0; i < (size_t)z_L; i++) {
        uint64_t lo = (i + q < (size_t)z_L) ? x[i + q] : 0;
        uint64_t hi = (i + q + 1 < (size_t)z_L) ? x[i + q + 1] : 0;
        x[i] = b ? (uint32_t)((lo >> b) | (hi << (32 - b))) : (uint32_t)lo;
    }
}
static void z_shl(uint32_t *x, uint64_t s) {
    long long q = (long long)(s >> 5); int b = (int)(s & 31);
    for (long long i = z_L - 1; i >= 0; i--) {
        uint64_t lo = (i - q >= 0) ? x[i - q] : 0;
        uint64_t lo2 = (i - q - 1 >= 0) ? x[i - q - 1] : 0;
        x[i] = b ? (uint32_t)((lo << b) | (lo2 >> (32 - b))) : (uint32_t)lo;
    }
}
static void z_addmul(uint32_t *acc, const uint32_t *X, const uint32_t *G) {
    int gl = z_L, xl = z_L;
    while (gl > 0 && G[gl - 1] == 0) gl--;
    while (xl > 0 && X[xl - 1] == 0) xl--;
    for (int j = 0; j < gl; j++) {
        if (G[j] == 0) continue;
        uint64_t c = 0;
        int i;
        for (i = 0; i < xl && i + j < z_L; i++) {
            uint64_t t = (uint64_t)X[i] * G[j] + acc[i + j] + c;
            acc[i + j] = (uint32_t)t; c = t >> 32;
        }
        int k = i + j;
        while (c && k < z_L) { uint64_t t = (uint64_t)acc[k] + c; acc[k] = (uint32_t)t; c = t >> 32; k++; }
    }
}

static void z_cent(int d, const int *p, int len, uint64_t *sh) {
    long long s2 = 0;
    for (int i = 1; i <= p[0]; i++) {
        long long c = 0;
        for (int j = 0; j < len; j++) if (p[j] >= i) c++;
        s2 += c * c;
    }
    int j = 0;
    while (j < len) {
        int k = j;
        while (k < len && p[k] == p[j]) k++;
        int mu = k - j;
        s2 -= (long long)mu * (mu + 1) / 2;
        for (int t = 1; t <= mu; t++) {
            uint32_t dv = (uint32_t)((1ull << (d * t)) - 1);
            if (dv > 1) z_div(z_G, dv);
        }
        j = k;
    }
    *sh += (uint64_t)((long long)d * s2);
}

static int z_fexp(uint64_t k, int r) {
    uint64_t K = k & (~k + 1);
    int ex = 0;
    if (z_ulen > 0) {
        if (K <= (uint64_t)r) return -1;
        for (int j = 0; j < z_ulen; j++) ex += ((uint64_t)z_ulam[j] < K) ? z_ulam[j] : (int)K;
    }
    for (int c = 0; c < z_ncomp; c++) {
        if (k % z_ce[c] == 0) {
            int s = 0;
            for (int j = 0; j < z_clen[c]; j++) s += ((uint64_t)z_cpart[c][j] < K) ? z_cpart[c][j] : (int)K;
            ex += z_cd[c] * s;
        }
    }
    return ex;
}

static void z_leaf(void) {
    int n = z_n;
    uint64_t sh = 0;
    memcpy(z_G, z_glodd, (size_t)z_L * sizeof(uint32_t));
    if (z_ulen) z_cent(1, z_ulam, z_ulen, &sh);
    for (int c = 0; c < z_ncomp; c++) z_cent(z_cd[c], z_cpart[c], z_clen[c], &sh);
    z_shl(z_G, (uint64_t)n * (n - 1) / 2 - sh);
    int maxp = 0;
    if (z_ulen) maxp = z_ulam[0];
    uint64_t el = 1;
    for (int c = 0; c < z_ncomp; c++) {
        if (z_cpart[c][0] > maxp) maxp = z_cpart[c][0];
        el = el / z_gcd(el, z_ce[c]) * z_ce[c];
    }
    for (int ri = -1; ri < z_ulen; ri++) {
        int r, expN; uint64_t fac;
        if (ri < 0) { r = 0; expN = z_m - z_ulen; fac = 1; }
        else {
            if (ri > 0 && z_ulam[ri] == z_ulam[ri - 1]) continue;
            r = z_ulam[ri];
            int mu = 0;
            while (ri + mu < z_ulen && z_ulam[ri + mu] == r) mu++;
            expN = z_m - ri - mu;
            fac = (1ull << mu) - 1;
        }
        int need = maxp > r + 1 ? maxp : r + 1;
        int S = 0;
        while ((1 << S) < need) S++;
        uint64_t M = el << S;
        uint64_t pr[64]; int pa[64]; int np = 0;
        uint64_t t = M;
        for (uint64_t p = 2; p * p <= t; p++) {
            if (t % p == 0) { pr[np] = p; pa[np] = 0; while (t % p == 0) { t /= p; pa[np]++; } np++; }
        }
        if (t > 1) { pr[np] = t; pa[np] = 1; np++; }
        long long D = 1;
        for (int i = 0; i < np; i++) D *= (pa[i] + 1);
        uint64_t *dv = (uint64_t *)malloc((size_t)D * sizeof(uint64_t));
        long long *F = (long long *)malloc((size_t)D * sizeof(long long));
        long long cnt = 1;
        dv[0] = 1;
        for (int i = 0; i < np; i++) {
            long long old = cnt;
            uint64_t pw = 1;
            for (int a = 1; a <= pa[i]; a++) {
                pw *= pr[i];
                for (long long q = 0; q < old; q++) dv[cnt++] = dv[q] * pw;
            }
        }
        for (long long q = 0; q < D; q++) {
            int e = z_fexp(dv[q], r);
            F[q] = e < 0 ? 0 : (1LL << e);
        }
        long long C = 0, Co = 0, C3 = 0, Cn4 = 0;
        for (long long q = 0; q < D; q++) {
            uint64_t Lc = dv[q];
            long long s = 0;
            for (long long w = 0; w < D; w++) {
                if (Lc % dv[w]) continue;
                uint64_t x = Lc / dv[w];
                int mu = 1;
                for (int i = 0; i < np && mu; i++) {
                    if (x % pr[i] == 0) { x /= pr[i]; if (x % pr[i] == 0) mu = 0; else mu = -mu; }
                }
                s += mu * F[w];
            }
            long long cL = s / (long long)Lc;
            if (cL == 0) continue;
            C += cL;
            if (Lc % 2) Co += cL;
            if (Lc % 3 == 0) C3 += cL;
            if (Lc % 4) Cn4 += cL;
        }
        free(dv); free(F);
        memset(z_Y, 0, (size_t)z_L * sizeof(uint32_t));
        uint64_t base = (uint64_t)(n - z_m + expN);
        z_shl_add(z_Y, base + 2 * C, fac);
        z_shl_add(z_Y, base + 2 * (C - Co) + Co, 6 * fac);
        if (Co == 0) z_shl_add(z_Y, base + 2 * C, 3 * fac);
        z_shl_add(z_Y, base + 2 * C3, 8 * fac);
        if (Cn4 == 0) z_shl_add(z_Y, base + 2 * C, 6 * fac);
        z_addmul(z_tot, z_Y, z_G);
    }
}

static void z_rec(int start, int rem) {
    if (rem == 0) { z_leaf(); return; }
    for (int p = start; p < z_T; p++) {
        int d = z_pd[p];
        if (d > rem) break;
        for (int j = 1; j * d <= rem; j++) {
            int *pp = z_cpart[z_ncomp];
            int len = 1;
            pp[0] = j;
            do {
                z_cd[z_ncomp] = d; z_ce[z_ncomp] = z_pe[p]; z_clen[z_ncomp] = len;
                z_ncomp++;
                z_rec(p + 1, rem - j * d);
                z_ncomp--;
            } while (z_next_part(pp, &len));
        }
    }
}

seq_t a(idx_t n) {
    if (n < 1 || n > 26) return 0;
    int N = (int)n;
    z_n = N;
    /* irreducible polynomials over GF(2) of degree 2..N with their orders */
    size_t cap = ((size_t)1 << N) + 4;
    uint64_t *poly = (uint64_t *)malloc(cap * sizeof(uint64_t));
    z_pd = (int *)malloc(cap * sizeof(int));
    z_pe = (uint64_t *)malloc(cap * sizeof(uint64_t));
    z_T = 0;
    for (int d = 2; d <= N; d++) {
        for (uint64_t f = (1ull << d) | 1; f < (2ull << d); f += 2) {
            if (__builtin_popcountll(f) % 2 == 0) continue;
            int ok = 1;
            for (int i = 0; i < z_T && 2 * z_pd[i] <= d; i++)
                if (z_pmod(f, poly[i]) == 0) { ok = 0; break; }
            if (!ok) continue;
            uint64_t cur = 1, k = 0;
            do { k++; cur <<= 1; if (cur >> d) cur ^= f; } while (cur != 1);
            poly[z_T] = f; z_pd[z_T] = d; z_pe[z_T] = k; z_T++;
        }
    }
    z_L = (int)((((uint64_t)1 << (N + 1)) + (uint64_t)N * N + 4 * (uint64_t)N + 96) / 32 + 4);
    z_glodd = (uint32_t *)calloc((size_t)z_L, sizeof(uint32_t));
    z_G = (uint32_t *)calloc((size_t)z_L, sizeof(uint32_t));
    z_Y = (uint32_t *)calloc((size_t)z_L, sizeof(uint32_t));
    z_tot = (uint32_t *)calloc((size_t)z_L, sizeof(uint32_t));
    z_glodd[0] = 1;
    for (int i = 2; i <= N; i++) z_mul(z_glodd, (uint32_t)((1u << i) - 1));
    z_cd = (int *)malloc((size_t)(N + 1) * sizeof(int));
    z_ce = (uint64_t *)malloc((size_t)(N + 1) * sizeof(uint64_t));
    z_clen = (int *)malloc((size_t)(N + 1) * sizeof(int));
    z_cpart = (int **)malloc((size_t)(N + 1) * sizeof(int *));
    for (int i = 0; i <= N; i++) z_cpart[i] = (int *)malloc((size_t)(N + 1) * sizeof(int));
    z_ulam = (int *)malloc((size_t)(N + 1) * sizeof(int));
    z_ncomp = 0;
    for (int m = 0; m <= N; m++) {
        z_m = m;
        if (m == 0) { z_ulen = 0; z_rec(0, N); continue; }
        z_ulen = 1; z_ulam[0] = m;
        do { z_rec(0, N - m); } while (z_next_part(z_ulam, &z_ulen));
    }
    z_shr(z_tot, (uint64_t)N + (uint64_t)N * (N - 1) / 2 + 3);
    z_div(z_tot, 3);
    for (int i = 2; i <= N; i++) z_div(z_tot, (uint32_t)((1u << i) - 1));
    seq_t v = 0;
    for (int i = z_L - 1; i >= 0; i--)
        for (int s = 24; s >= 0; s -= 8) {
            v = v * 256u;
            v = v + (unsigned)((z_tot[i] >> s) & 255u);
        }
    for (int i = 0; i <= N; i++) free(z_cpart[i]);
    free(z_cpart); free(z_cd); free(z_ce); free(z_clen); free(z_ulam);
    free(z_glodd); free(z_G); free(z_Y); free(z_tot);
    free(poly); free(z_pd); free(z_pe);
    return v;
}
