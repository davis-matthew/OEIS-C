/*
 * A001537  Invertible Boolean functions with AG(n,2) acting on the domain and
 *          range.
 * https://oeis.org/A001537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/|AGL(n,2)|^2) * Sum_{lambda} c(lambda)^2 * z(lambda)
 *            (Burnside's lemma for AGL(n,2) x AGL(n,2) acting on the
 *            permutations of GF(2)^n by f -> g f h^-1). Here c(lambda) is the
 *            number of elements of AGL(n,2) whose cycle type on the 2^n
 *            points is lambda, and z(lambda) = Prod_j j^m_j * m_j!....
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   x1537_deg, x1537_pmod, x1537_pmul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/x1537_deg.h"
#include "../utils/x1537_pmod.h"
#include "../utils/x1537_pmul.h"

typedef struct { uint32_t *d; size_t n, cap; } Z1537;

static void z1537_reserve(Z1537 *a, size_t c)
{
    if (c > a->cap) {
        size_t nc = a->cap ? a->cap : 4;
        while (nc < c) nc *= 2;
        a->d = (uint32_t *)realloc(a->d, nc * sizeof(uint32_t));
        a->cap = nc;
    }
}
static void z1537_trim(Z1537 *a) { while (a->n && a->d[a->n - 1] == 0) a->n--; }
static void z1537_set(Z1537 *a, uint32_t v)
{
    z1537_reserve(a, 1);
    a->d[0] = v;
    a->n = v ? 1 : 0;
}
static void z1537_mulsmall(Z1537 *a, uint32_t m)
{
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < a->n; i++) {
        c += (uint64_t)a->d[i] * m;
        a->d[i] = (uint32_t)c;
        c >>= 32;
    }
    if (c) { z1537_reserve(a, a->n + 1); a->d[a->n++] = (uint32_t)c; }
    z1537_trim(a);
}
static void z1537_divsmall(Z1537 *a, uint32_t m)
{
    uint64_t r = 0;
    size_t i;
    for (i = a->n; i-- > 0;) {
        r = (r << 32) | a->d[i];
        a->d[i] = (uint32_t)(r / m);
        r %= m;
    }
    z1537_trim(a);
}
static void z1537_shl(Z1537 *a, unsigned s)
{
    size_t w = s / 32, i;
    unsigned b = s % 32;
    if (a->n == 0) return;
    z1537_reserve(a, a->n + w + 1);
    a->d[a->n + w] = 0;
    for (i = a->n; i-- > 0;) {
        uint64_t v = (uint64_t)a->d[i] << b;
        a->d[i + w + 1] |= (uint32_t)(v >> 32);
        a->d[i + w] = (uint32_t)v;
    }
    for (i = 0; i < w; i++) a->d[i] = 0;
    a->n += w + 1;
    z1537_trim(a);
}
static void z1537_add(Z1537 *a, const Z1537 *b)
{
    size_t m = a->n > b->n ? a->n : b->n, i;
    uint64_t c = 0;
    z1537_reserve(a, m + 1);
    for (i = a->n; i < m + 1; i++) a->d[i] = 0;
    for (i = 0; i < m; i++) {
        c += (uint64_t)a->d[i] + (i < b->n ? b->d[i] : 0);
        a->d[i] = (uint32_t)c;
        c >>= 32;
    }
    a->d[m] = (uint32_t)c;
    a->n = m + 1;
    z1537_trim(a);
}
static void z1537_mul(Z1537 *r, const Z1537 *a, const Z1537 *b)
{
    size_t i, j;
    if (a->n == 0 || b->n == 0) { r->n = 0; return; }
    z1537_reserve(r, a->n + b->n);
    for (i = 0; i < a->n + b->n; i++) r->d[i] = 0;
    for (i = 0; i < a->n; i++) {
        uint64_t c = 0;
        for (j = 0; j < b->n; j++) {
            c += (uint64_t)a->d[i] * b->d[j] + r->d[i + j];
            r->d[i + j] = (uint32_t)c;
            c >>= 32;
        }
        r->d[i + b->n] = (uint32_t)c;
    }
    r->n = a->n + b->n;
    z1537_trim(r);
}

static int g1537_n;
static uint32_t g1537_N;
static uint64_t *g1537_poly;
static int *g1537_pdeg;
static int g1537_np;
static int g1537_bp[64], g1537_bk[64], g1537_nb;
static uint32_t *g1537_tab, *g1537_vis, *g1537_cnt, *g1537_touch, *g1537_key, g1537_stamp;
static Z1537 g1537_w;
/* hash table of cycle types */
static long *g1537_ht;
static size_t g1537_hcap, g1537_ne, g1537_ecap;
static size_t *g1537_eko;
static uint32_t *g1537_ekl;
static uint64_t *g1537_eh;
static Z1537 *g1537_ew;
static uint32_t *g1537_pool;
static size_t g1537_pn, g1537_pcap;

static void x1537_rehash(void)
{
    size_t nc = g1537_hcap * 2, i, pos;
    long *nt = (long *)malloc(nc * sizeof(long));
    for (i = 0; i < nc; i++) nt[i] = -1;
    for (i = 0; i < g1537_ne; i++) {
        pos = (size_t)(g1537_eh[i] & (nc - 1));
        while (nt[pos] >= 0) pos = (pos + 1) & (nc - 1);
        nt[pos] = (long)i;
    }
    free(g1537_ht);
    g1537_ht = nt;
    g1537_hcap = nc;
}

static void x1537_insert(const uint32_t *key, uint32_t klen, const Z1537 *w)
{
    uint64_t h = 1469598103934665603ULL;
    uint32_t i;
    size_t pos, e;
    for (i = 0; i < klen; i++) { h ^= key[i]; h *= 1099511628211ULL; }
    if (2 * (g1537_ne + 1) > g1537_hcap) x1537_rehash();
    pos = (size_t)(h & (g1537_hcap - 1));
    while (g1537_ht[pos] >= 0) {
        e = (size_t)g1537_ht[pos];
        if (g1537_eh[e] == h && g1537_ekl[e] == klen &&
            memcmp(g1537_pool + g1537_eko[e], key, klen * sizeof(uint32_t)) == 0) {
            z1537_add(&g1537_ew[e], w);
            return;
        }
        pos = (pos + 1) & (g1537_hcap - 1);
    }
    if (g1537_ne == g1537_ecap) {
        size_t nc = g1537_ecap ? g1537_ecap * 2 : 256, k;
        g1537_eko = (size_t *)realloc(g1537_eko, nc * sizeof(size_t));
        g1537_ekl = (uint32_t *)realloc(g1537_ekl, nc * sizeof(uint32_t));
        g1537_eh = (uint64_t *)realloc(g1537_eh, nc * sizeof(uint64_t));
        g1537_ew = (Z1537 *)realloc(g1537_ew, nc * sizeof(Z1537));
        for (k = g1537_ecap; k < nc; k++) { g1537_ew[k].d = NULL; g1537_ew[k].n = 0; g1537_ew[k].cap = 0; }
        g1537_ecap = nc;
    }
    if (g1537_pn + klen > g1537_pcap) {
        size_t nc = g1537_pcap ? g1537_pcap : 1024;
        while (nc < g1537_pn + klen) nc *= 2;
        g1537_pool = (uint32_t *)realloc(g1537_pool, nc * sizeof(uint32_t));
        g1537_pcap = nc;
    }
    e = g1537_ne++;
    memcpy(g1537_pool + g1537_pn, key, klen * sizeof(uint32_t));
    g1537_eko[e] = g1537_pn;
    g1537_pn += klen;
    g1537_ekl[e] = klen;
    g1537_eh[e] = h;
    g1537_ew[e].n = 0;
    z1537_add(&g1537_ew[e], w);
    g1537_ht[pos] = (long)e;
}

static void x1537_process(void)
{
    int n = g1537_n, pos = 0, ell = 0, i, j, t, u;
    uint32_t col[32], e0[32], x, N = g1537_N, mask;
    long e2 = 0;
    for (i = 0; i < g1537_nb; i++) {
        uint64_t f = g1537_poly[g1537_bp[i]], g = 1;
        int D = g1537_pdeg[g1537_bp[i]] * g1537_bk[i];
        for (j = 0; j < g1537_bk[i]; j++) g = x1537_pmul(g, f);
        for (j = 0; j < D - 1; j++) col[pos + j] = 1u << (pos + j + 1);
        col[pos + D - 1] = 0;
        for (j = 0; j < D; j++)
            if ((g >> j) & 1) col[pos + D - 1] |= 1u << (pos + j);
        if (f == 3) e0[ell++] = 1u << pos;
        pos += D;
    }
    /* class size of A in GL(n,2) times 2^(n-ell) translations per representative */
    z1537_set(&g1537_w, 1);
    for (i = 1; i <= n; i++) z1537_mulsmall(&g1537_w, (1u << i) - 1);
    i = 0;
    while (i < g1537_nb) {
        int p = g1537_bp[i], d = g1537_pdeg[p], s = i, maxpart;
        long sq = 0, mm = 0;
        while (i < g1537_nb && g1537_bp[i] == p) i++;
        maxpart = g1537_bk[s];
        for (t = 1; t <= maxpart; t++) {
            long c = 0;
            for (u = s; u < i; u++) if (g1537_bk[u] >= t) c++;
            sq += c * c;
        }
        u = s;
        while (u < i) {
            int v = u;
            long m;
            while (v < i && g1537_bk[v] == g1537_bk[u]) v++;
            m = v - u;
            mm += m * (m + 1) / 2;
            for (t = 1; t <= m; t++) z1537_divsmall(&g1537_w, (uint32_t)((1ULL << (d * t)) - 1));
            u = v;
        }
        e2 += (long)d * (sq - mm);
    }
    z1537_shl(&g1537_w, (unsigned)((long)n * (n - 1) / 2 - e2 + n - ell));

    g1537_tab[0] = 0;
    for (x = 1; x < N; x++) g1537_tab[x] = g1537_tab[x & (x - 1)] ^ col[__builtin_ctz(x)];

    for (mask = 0; mask < (1u << ell); mask++) {
        uint32_t b = 0, nt = 0, k;
        for (j = 0; j < ell; j++) if ((mask >> j) & 1) b |= e0[j];
        g1537_stamp++;
        for (x = 0; x < N; x++) {
            if (g1537_vis[x] != g1537_stamp) {
                uint32_t y = x, len = 0;
                do { g1537_vis[y] = g1537_stamp; y = g1537_tab[y] ^ b; len++; } while (y != x);
                if (g1537_cnt[len]++ == 0) g1537_touch[nt++] = len;
            }
        }
        for (k = 1; k < nt; k++) {
            uint32_t v = g1537_touch[k], q = k;
            while (q > 0 && g1537_touch[q - 1] > v) { g1537_touch[q] = g1537_touch[q - 1]; q--; }
            g1537_touch[q] = v;
        }
        for (k = 0; k < nt; k++) {
            g1537_key[2 * k] = g1537_touch[k];
            g1537_key[2 * k + 1] = g1537_cnt[g1537_touch[k]];
            g1537_cnt[g1537_touch[k]] = 0;
        }
        x1537_insert(g1537_key, 2 * nt, &g1537_w);
    }
}

static void x1537_rec(int start, int rem);
static void x1537_parts(int idx, int rem, int maxp)
{
    int d = g1537_pdeg[idx], k;
    for (k = 1; k <= maxp && k * d <= rem; k++) {
        g1537_bp[g1537_nb] = idx;
        g1537_bk[g1537_nb] = k;
        g1537_nb++;
        x1537_rec(idx + 1, rem - k * d);
        x1537_parts(idx, rem - k * d, k);
        g1537_nb--;
    }
}
static void x1537_rec(int start, int rem)
{
    int idx;
    if (rem == 0) { x1537_process(); return; }
    for (idx = start; idx < g1537_np && g1537_pdeg[idx] <= rem; idx++)
        x1537_parts(idx, rem, rem / g1537_pdeg[idx]);
}

static void x1537_compute(int n, Z1537 *out)
{
    uint32_t N = 1u << n;
    size_t i;
    int d, q, rep;
    uint64_t p;
    Z1537 S = {NULL, 0, 0}, t = {NULL, 0, 0}, u = {NULL, 0, 0}, z = {NULL, 0, 0};

    g1537_n = n;
    g1537_N = N;
    g1537_poly = (uint64_t *)malloc(((size_t)2 << n) * sizeof(uint64_t));
    g1537_pdeg = (int *)malloc(((size_t)2 << n) * sizeof(int));
    g1537_np = 0;
    for (d = 1; d <= n; d++) {
        for (p = 1ULL << d; p < (2ULL << d); p++) {
            int ok = 1;
            if (!(p & 1)) continue;
            for (q = 0; q < g1537_np && 2 * g1537_pdeg[q] <= d; q++)
                if (x1537_pmod(p, g1537_poly[q]) == 0) { ok = 0; break; }
            if (ok) { g1537_poly[g1537_np] = p; g1537_pdeg[g1537_np] = d; g1537_np++; }
        }
    }
    g1537_tab = (uint32_t *)malloc((size_t)N * sizeof(uint32_t));
    g1537_vis = (uint32_t *)calloc((size_t)N, sizeof(uint32_t));
    g1537_cnt = (uint32_t *)calloc((size_t)N + 1, sizeof(uint32_t));
    g1537_touch = (uint32_t *)malloc(((size_t)N + 1) * sizeof(uint32_t));
    g1537_key = (uint32_t *)malloc(((size_t)N + 1) * 2 * sizeof(uint32_t));
    g1537_stamp = 0;
    g1537_hcap = 1024;
    g1537_ht = (long *)malloc(g1537_hcap * sizeof(long));
    for (i = 0; i < g1537_hcap; i++) g1537_ht[i] = -1;
    g1537_ne = 0;
    g1537_pn = 0;
    g1537_nb = 0;

    x1537_rec(0, n);

    z1537_set(&S, 0);
    for (i = 0; i < g1537_ne; i++) {
        const uint32_t *key = g1537_pool + g1537_eko[i];
        uint32_t kl = g1537_ekl[i], k, r;
        uint64_t acc = 1;
        z1537_set(&z, 1);
        for (k = 0; k < kl; k += 2) {
            uint32_t L = key[k], m = key[k + 1];
            for (r = 1; r <= m; r++) {
                if (acc * L >= (1ULL << 32)) { z1537_mulsmall(&z, (uint32_t)acc); acc = L; }
                else acc *= L;
                if (acc * r >= (1ULL << 32)) { z1537_mulsmall(&z, (uint32_t)acc); acc = r; }
                else acc *= r;
            }
        }
        z1537_mulsmall(&z, (uint32_t)acc);
        z1537_mul(&t, &g1537_ew[i], &g1537_ew[i]);
        z1537_mul(&u, &t, &z);
        z1537_add(&S, &u);
    }
    for (rep = 0; rep < 2; rep++) {
        z1537_divsmall(&S, N);
        for (d = 0; d < n; d++) z1537_divsmall(&S, N - (1u << d));
    }
    *out = S;

    for (i = 0; i < g1537_ecap; i++) { free(g1537_ew[i].d); }
    free(g1537_ew); g1537_ew = NULL;
    free(g1537_eko); g1537_eko = NULL;
    free(g1537_ekl); g1537_ekl = NULL;
    free(g1537_eh); g1537_eh = NULL;
    g1537_ecap = 0;
    free(g1537_pool); g1537_pool = NULL; g1537_pcap = 0;
    free(g1537_ht); g1537_ht = NULL;
    free(g1537_tab); free(g1537_vis); free(g1537_cnt); free(g1537_touch); free(g1537_key);
    free(g1537_poly); free(g1537_pdeg);
    free(t.d); free(u.d); free(z.d);
}

static Z1537 g1537_cache[32];
static int g1537_have[32];

seq_t a(idx_t n)
{
    Z1537 *r;
    seq_t v = 0;
    size_t i;
    int sh;
    if (n < 1 || n > 31) return v;
    if (!g1537_have[n]) {
        x1537_compute((int)n, &g1537_cache[n]);
        g1537_have[n] = 1;
    }
    r = &g1537_cache[n];
    for (i = r->n; i-- > 0;) {
        for (sh = 24; sh >= 0; sh -= 8) {
            v *= 256;
            v += (idx_t)((r->d[i] >> sh) & 255u);
        }
    }
    return v;
}
