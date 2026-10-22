/*
 * A000889  Number of switching networks with S(n) and C(2,2) acting on the
 *          domain and AG(2,2) acting on the range.
 * https://oeis.org/A000889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside's lemma: a(n) = (1/(2^n n! * 24)) * Sum_{g in B_n}
 *            Sum_{h in S_4} Prod_{cycles C of g on {0,1}^n} #{y : h^|C| y =
 *            y}, where B_n (S(n) with complementations) acts on the 2^n
 *            domain points and AG(2,2)=S_4 acts on the 4 range points; summed
 *            over conjugacy classes of B_n (signed cycle types)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   z889_fexp, z889_gcd, z889_mu, z889_v2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/z889_fexp.h"
#include "../utils/z889_gcd.h"
#include "../utils/z889_mu.h"
#include "../utils/z889_v2.h"

static uint32_t *z889_Z;
static int z889_n, z889_nc;
static int z889_k[64], z889_s[64];
static unsigned __int128 z889_fact;

static void z889_add(size_t pos, uint64_t v) {
    uint64_t c = v;
    while (c) {
        uint64_t s = (uint64_t)z889_Z[pos] + (c & 0xffffffffULL);
        z889_Z[pos] = (uint32_t)s;
        c = (c >> 32) + (s >> 32);
        pos++;
    }
}

static void z889_process(unsigned __int128 den) {
    static const int w[5] = {1, 6, 8, 3, 6};
    unsigned __int128 size = z889_fact / den;
    long long M = 1, d;
    int i, j, nd = 0, h;
    long long *dv, *fx, *cc;
    for (i = 0; i < z889_nc; i++) {
        long long p = z889_s[i] ? 2LL * z889_k[i] : (long long)z889_k[i];
        M = M / z889_gcd(M, p) * p;
    }
    for (d = 1; d <= M; d++) if (M % d == 0) nd++;
    dv = (long long *)malloc(sizeof(long long) * nd);
    fx = (long long *)malloc(sizeof(long long) * nd);
    cc = (long long *)malloc(sizeof(long long) * nd);
    nd = 0;
    for (d = 1; d <= M; d++) if (M % d == 0) dv[nd++] = d;
    for (i = 0; i < nd; i++) {
        int e = 0, zero = 0;
        d = dv[i];
        for (j = 0; j < z889_nc; j++) {
            long long k = z889_k[j];
            if (z889_s[j] && !(z889_v2(d) > z889_v2(k))) { zero = 1; break; }
            e += (int)z889_gcd(d, k);
        }
        fx[i] = zero ? 0 : (1LL << e);
    }
    for (i = 0; i < nd; i++) {
        long long L = dv[i], s = 0;
        for (j = 0; j <= i; j++)
            if (L % dv[j] == 0) s += (long long)z889_mu(L / dv[j]) * fx[j];
        cc[i] = s / L;
    }
    for (h = 0; h < 5; h++) {
        uint64_t e = 0;
        int ok = 1, q;
        for (i = 0; i < nd; i++) {
            int f;
            if (cc[i] <= 0) continue;
            f = z889_fexp(h, dv[i]);
            if (f < 0) { ok = 0; break; }
            e += (uint64_t)cc[i] * (uint64_t)f;
        }
        if (ok) {
            unsigned __int128 coef = size * (unsigned __int128)w[h];
            size_t pos = (size_t)(e / 32);
            int b = (int)(e % 32);
            for (q = 0; q < 4; q++) {
                uint64_t piece = (uint64_t)(coef & 0xffffffffULL);
                coef >>= 32;
                if (piece) z889_add(pos + q, piece << b);
            }
        }
    }
    free(dv); free(fx); free(cc);
}

static void z889_rec(int t, int rem, unsigned __int128 den) {
    int k, sg, m;
    if (rem == 0) { z889_process(den); return; }
    if (t >= 2 * z889_n) return;
    k = t / 2 + 1;
    sg = t % 2;
    if (k > rem) return;
    for (m = 0; m * k <= rem; m++) {
        if (m > 0) {
            z889_k[z889_nc] = k;
            z889_s[z889_nc] = sg;
            z889_nc++;
            den *= (unsigned __int128)m * (unsigned __int128)(2 * k);
        }
        z889_rec(t + 1, rem - m * k, den);
    }
    z889_nc -= m - 1;
}

static void z889_div(size_t L, uint32_t dvs) {
    uint64_t r = 0;
    size_t i = L;
    while (i > 0) {
        uint64_t cur;
        i--;
        cur = (r << 32) | z889_Z[i];
        z889_Z[i] = (uint32_t)(cur / dvs);
        r = cur % dvs;
    }
}

seq_t a(idx_t n) {
    size_t L, i;
    int j;
    seq_t r = 0;
    if (n == 0) return r;
    z889_n = (int)n;
    z889_nc = 0;
    L = (((size_t)1 << (n + 1)) + 256) / 32 + 8;
    z889_Z = (uint32_t *)calloc(L, sizeof(uint32_t));
    z889_fact = 1;
    for (j = 1; j <= (int)n; j++) z889_fact *= (unsigned __int128)(2 * j);
    z889_rec(0, (int)n, 1);
    for (j = 1; j <= (int)n; j++) z889_div(L, 2);
    for (j = 2; j <= (int)n; j++) z889_div(L, (uint32_t)j);
    z889_div(L, 24);
    i = L;
    while (i > 0) {
        i--;
        r = r * (seq_t)65536 + (seq_t)(int)(z889_Z[i] >> 16);
        r = r * (seq_t)65536 + (seq_t)(int)(z889_Z[i] & 0xffff);
    }
    free(z889_Z);
    return r;
}
