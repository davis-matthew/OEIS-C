/*
 * A001038  Invertible Boolean functions with GL(n,2) acting on the domain and
 *          range.
 * https://oeis.org/A001038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside count of GL(n,2) x GL(n,2) orbits on the permutations
 *            of F_2^n: a(n) = (1/|GL(n,2)|^2) * Sum_{cycle types t} N(t)^2 *
 *            z(t), where N(t) = number of matrices in GL(n,2) whose action on
 *            the 2^n vectors has cycle type t, and z(t) = Prod_k k^{c_k} c_k!
 *            is the centralizer size in S_{2^n}. N(t)...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd_u64.h"

static void x1038_add(uint32_t *a, int *alen, const uint32_t *b, int blen)
{
    int L = *alen > blen ? *alen : blen, i;
    uint64_t carry = 0;
    for (i = *alen; i < L; i++) a[i] = 0;
    for (i = 0; i < L; i++) {
        uint64_t t = (uint64_t)a[i] + (i < blen ? b[i] : 0) + carry;
        a[i] = (uint32_t)t;
        carry = t >> 32;
    }
    if (carry) a[L++] = (uint32_t)carry;
    *alen = L;
}

static void x1038_divsmall(uint32_t *x, int *len, uint32_t d)
{
    uint64_t r = 0;
    int i;
    for (i = *len - 1; i >= 0; i--) {
        uint64_t t = (r << 32) | x[i];
        x[i] = (uint32_t)(t / d);
        r = t % d;
    }
    while (*len > 1 && x[*len - 1] == 0) (*len)--;
}

static int x1038_mul(const uint32_t *a, int al, const uint32_t *b, int bl, uint32_t *out)
{
    int i, j, len = al + bl;
    memset(out, 0, sizeof(uint32_t) * (size_t)len);
    for (i = 0; i < al; i++) {
        uint64_t carry = 0;
        for (j = 0; j < bl; j++) {
            uint64_t t = (uint64_t)a[i] * b[j] + out[i + j] + carry;
            out[i + j] = (uint32_t)t;
            carry = t >> 32;
        }
        out[i + bl] = (uint32_t)carry;
    }
    while (len > 1 && out[len - 1] == 0) len--;
    return len;
}

static void x1038_mulsmall(uint32_t *x, int *len, uint32_t m)
{
    uint64_t carry = 0;
    int i;
    for (i = 0; i < *len; i++) {
        uint64_t t = (uint64_t)x[i] * m + carry;
        x[i] = (uint32_t)t;
        carry = t >> 32;
    }
    if (carry) x[(*len)++] = (uint32_t)carry;
}

static uint64_t x1038_pmulmod(uint64_t a, uint64_t b, uint64_t f, int d)
{
    uint64_t r = 0;
    int i;
    for (i = d - 1; i >= 0; i--) {
        r <<= 1;
        if ((r >> d) & 1) r ^= f;
        if ((b >> i) & 1) r ^= a;
    }
    return r;
}

static uint64_t x1038_xpow(uint64_t e, uint64_t f, int d)
{
    uint64_t res = 1, base = (d > 1) ? 2 : 1;
    while (e) {
        if (e & 1) res = x1038_pmulmod(res, base, f, d);
        base = x1038_pmulmod(base, base, f, d);
        e >>= 1;
    }
    return res;
}

static uint64_t x1038_order(uint64_t f, int d)
{
    uint64_t M, m, ord, p;
    if (d == 1) return 1;
    M = (1ULL << d) - 1;
    m = M;
    ord = M;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            while (ord % p == 0 && x1038_xpow(ord / p, f, d) == 1) ord /= p;
        }
    }
    if (m > 1)
        while (ord % m == 0 && x1038_xpow(ord / m, f, d) == 1) ord /= m;
    return ord;
}

static uint64_t x1038_pmod(uint64_t a, uint64_t h, int dh)
{
    int i;
    for (i = 63; i >= dh; i--)
        if ((a >> i) & 1) a ^= h << (i - dh);
    return a;
}

static int g_n;
static int g_ni, g_icap;
static uint64_t *g_ipoly, *g_iord;
static int *g_ideg;
static int *g_pp, *g_plen, g_pnum, g_pcap;
static int g_pstart[64], g_pcount[64];
static int g_sti[64], g_stp[64], g_sd;
static uint32_t *g_G;
static int g_Glen, g_capN;
static uint64_t **g_tkey;
static int *g_tklen;
static uint32_t **g_tN;
static int *g_tNlen;
static int g_nt, g_tcap;

static void x1038_genpart(int maxp, int *cur, int len, int rem)
{
    int p;
    if (rem == 0) {
        if (g_pnum >= g_pcap) {
            g_pcap = g_pcap ? 2 * g_pcap : 64;
            g_pp = (int *)realloc(g_pp, sizeof(int) * (size_t)g_pcap * (size_t)g_n);
            g_plen = (int *)realloc(g_plen, sizeof(int) * (size_t)g_pcap);
        }
        memcpy(g_pp + (size_t)g_pnum * g_n, cur, sizeof(int) * (size_t)len);
        g_plen[g_pnum++] = len;
        return;
    }
    for (p = (maxp < rem ? maxp : rem); p >= 1; p--) {
        cur[len] = p;
        x1038_genpart(p, cur, len + 1, rem - p);
    }
}

static void x1038_leaf(void)
{
    int n = g_n, k, a, e, j, nf = 0, dn = 1, ql, t;
    size_t cap = 1;
    uint64_t *dL, *dC, *nL, *nC, *key;
    uint64_t fac[64];
    long long e2 = 0;
    uint32_t *q;

    for (k = 0; k < g_sd; k++) cap *= (size_t)(g_pp[(size_t)g_stp[k] * n] + 1);
    dL = (uint64_t *)malloc(sizeof(uint64_t) * cap);
    dC = (uint64_t *)malloc(sizeof(uint64_t) * cap);
    nL = (uint64_t *)malloc(sizeof(uint64_t) * cap);
    nC = (uint64_t *)malloc(sizeof(uint64_t) * cap);
    dL[0] = 1; dC[0] = 1;

    for (k = 0; k < g_sd; k++) {
        int i = g_sti[k];
        const int *pt = g_pp + (size_t)g_stp[k] * n;
        int pl = g_plen[g_stp[k]], d = g_ideg[i], l1 = pt[0], prevS = 0, nn = 0;
        uint64_t ord = g_iord[i], cnt[65], oo[65], *tp;
        long long sumsq = 0, sub = 0;
        cnt[0] = 1; oo[0] = 1;
        for (e = 1; e <= l1; e++) {
            int S = 0, tt = 0;
            for (j = 0; j < pl; j++) S += pt[j] < e ? pt[j] : e;
            cnt[e] = (1ULL << (d * S)) - (1ULL << (d * prevS));
            prevS = S;
            while ((1 << tt) < e) tt++;
            oo[e] = ord << tt;
        }
        for (a = 0; a < dn; a++) {
            for (e = 0; e <= l1; e++) {
                uint64_t L = dL[a] / gcd_u64(dL[a], oo[e]) * oo[e];
                uint64_t c = dC[a] * cnt[e];
                int b;
                for (b = 0; b < nn; b++) if (nL[b] == L) break;
                if (b == nn) { nL[nn] = L; nC[nn] = 0; nn++; }
                nC[b] += c;
            }
        }
        tp = dL; dL = nL; nL = tp;
        tp = dC; dC = nC; nC = tp;
        dn = nn;
        for (j = 1; j <= l1; j++) {
            long long c = 0;
            int r;
            for (r = 0; r < pl; r++) if (pt[r] >= j) c++;
            sumsq += c * c;
        }
        j = 0;
        while (j < pl) {
            int v = pt[j], m = 0, kk;
            while (j < pl && pt[j] == v) { m++; j++; }
            sub += (long long)m * (m + 1) / 2;
            for (kk = 1; kk <= m; kk++) {
                uint64_t f = (1ULL << (d * kk)) - 1;
                if (f > 1) fac[nf++] = f;
            }
        }
        e2 += (long long)d * (sumsq - sub);
    }

    for (a = 0; a < dn; a++) dC[a] /= dL[a];
    for (a = 1; a < dn; a++) {
        uint64_t L = dL[a], C = dC[a];
        int b = a - 1;
        while (b >= 0 && dL[b] > L) { dL[b + 1] = dL[b]; dC[b + 1] = dC[b]; b--; }
        dL[b + 1] = L; dC[b + 1] = C;
    }
    key = (uint64_t *)malloc(sizeof(uint64_t) * 2 * (size_t)dn);
    for (a = 0; a < dn; a++) { key[2 * a] = dL[a]; key[2 * a + 1] = dC[a]; }
    free(dL); free(dC); free(nL); free(nC);

    for (t = 0; t < g_nt; t++)
        if (g_tklen[t] == dn && memcmp(g_tkey[t], key, sizeof(uint64_t) * 2 * (size_t)dn) == 0) break;
    if (t == g_nt) {
        if (g_nt >= g_tcap) {
            g_tcap = g_tcap ? 2 * g_tcap : 64;
            g_tkey = (uint64_t **)realloc(g_tkey, sizeof(uint64_t *) * (size_t)g_tcap);
            g_tklen = (int *)realloc(g_tklen, sizeof(int) * (size_t)g_tcap);
            g_tN = (uint32_t **)realloc(g_tN, sizeof(uint32_t *) * (size_t)g_tcap);
            g_tNlen = (int *)realloc(g_tNlen, sizeof(int) * (size_t)g_tcap);
        }
        g_tkey[t] = key;
        g_tklen[t] = dn;
        g_tN[t] = (uint32_t *)calloc((size_t)g_capN, sizeof(uint32_t));
        g_tNlen[t] = 1;
        g_nt++;
    } else {
        free(key);
    }

    q = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)g_capN);
    memcpy(q, g_G, sizeof(uint32_t) * (size_t)g_Glen);
    ql = g_Glen;
    while (e2 > 0) {
        int s = e2 > 30 ? 30 : (int)e2;
        x1038_divsmall(q, &ql, 1u << s);
        e2 -= s;
    }
    for (a = 0; a < nf; a++) x1038_divsmall(q, &ql, (uint32_t)fac[a]);
    x1038_add(g_tN[t], &g_tNlen[t], q, ql);
    free(q);
}

static void x1038_rec(int start, int rem)
{
    int i, s, p;
    if (rem == 0) { x1038_leaf(); return; }
    for (i = start; i < g_ni; i++) {
        int d = g_ideg[i];
        if (d > rem) break;
        for (s = 1; s * d <= rem; s++) {
            for (p = g_pstart[s]; p < g_pstart[s] + g_pcount[s]; p++) {
                g_sti[g_sd] = i; g_stp[g_sd] = p; g_sd++;
                x1038_rec(i + 1, rem - s * d);
                g_sd--;
            }
        }
    }
}

seq_t a(idx_t nn)
{
    int n = (int)nn, d, i, t, k, Slen, zlen, n2len, Tlen;
    int cur[64];
    long long e2;
    size_t capZ, capS;
    uint32_t *S, *z, *N2, *T;
    seq_t r = 0;

    g_n = n;
    g_ni = 0; g_icap = 0; g_ipoly = NULL; g_iord = NULL; g_ideg = NULL;
    for (d = 1; d <= n; d++) {
        uint64_t g;
        for (g = (1ULL << d) | 1; g < (1ULL << (d + 1)); g += 2) {
            int ok = 1;
            for (i = 0; i < g_ni && 2 * g_ideg[i] <= d; i++)
                if (x1038_pmod(g, g_ipoly[i], g_ideg[i]) == 0) { ok = 0; break; }
            if (!ok) continue;
            if (g_ni >= g_icap) {
                g_icap = g_icap ? 2 * g_icap : 64;
                g_ipoly = (uint64_t *)realloc(g_ipoly, sizeof(uint64_t) * (size_t)g_icap);
                g_iord = (uint64_t *)realloc(g_iord, sizeof(uint64_t) * (size_t)g_icap);
                g_ideg = (int *)realloc(g_ideg, sizeof(int) * (size_t)g_icap);
            }
            g_ipoly[g_ni] = g;
            g_ideg[g_ni] = d;
            g_iord[g_ni] = x1038_order(g, d);
            g_ni++;
        }
    }

    g_pp = NULL; g_plen = NULL; g_pnum = 0; g_pcap = 0;
    for (k = 1; k <= n; k++) {
        g_pstart[k] = g_pnum;
        x1038_genpart(k, cur, 0, k);
        g_pcount[k] = g_pnum - g_pstart[k];
    }

    g_capN = n * n / 32 + 8;
    g_G = (uint32_t *)calloc((size_t)g_capN, sizeof(uint32_t));
    g_G[0] = 1; g_Glen = 1;
    e2 = (long long)n * (n - 1) / 2;
    while (e2 > 0) {
        int s = e2 > 30 ? 30 : (int)e2;
        x1038_mulsmall(g_G, &g_Glen, 1u << s);
        e2 -= s;
    }
    for (k = 1; k <= n; k++) x1038_mulsmall(g_G, &g_Glen, (uint32_t)((1ULL << k) - 1));

    g_tkey = NULL; g_tklen = NULL; g_tN = NULL; g_tNlen = NULL; g_nt = 0; g_tcap = 0;
    g_sd = 0;
    x1038_rec(0, n);

    capZ = (((size_t)n << n) / 32) + 8;
    capS = capZ + 2 * (size_t)g_capN + 16;
    S = (uint32_t *)calloc(capS, sizeof(uint32_t));
    z = (uint32_t *)calloc(capZ, sizeof(uint32_t));
    N2 = (uint32_t *)calloc(2 * (size_t)g_capN + 2, sizeof(uint32_t));
    T = (uint32_t *)calloc(capS, sizeof(uint32_t));
    Slen = 1;
    for (t = 0; t < g_nt; t++) {
        uint64_t acc = 1;
        z[0] = 1; zlen = 1;
        for (i = 0; i < g_tklen[t]; i++) {
            uint64_t L = g_tkey[t][2 * i], c = g_tkey[t][2 * i + 1], j;
            for (j = 1; j <= c; j++) {
                uint64_t vals[2];
                int h;
                vals[0] = L; vals[1] = j;
                for (h = 0; h < 2; h++) {
                    uint64_t v = vals[h];
                    if (v == 1) continue;
                    if (acc * v >= (1ULL << 32)) {
                        x1038_mulsmall(z, &zlen, (uint32_t)acc);
                        acc = v;
                    } else {
                        acc *= v;
                    }
                }
            }
        }
        if (acc > 1) x1038_mulsmall(z, &zlen, (uint32_t)acc);
        n2len = x1038_mul(g_tN[t], g_tNlen[t], g_tN[t], g_tNlen[t], N2);
        Tlen = x1038_mul(z, zlen, N2, n2len, T);
        x1038_add(S, &Slen, T, Tlen);
    }

    e2 = (long long)n * (n - 1);
    while (e2 > 0) {
        int s = e2 > 30 ? 30 : (int)e2;
        x1038_divsmall(S, &Slen, 1u << s);
        e2 -= s;
    }
    for (k = 1; k <= n; k++) {
        uint32_t f = (uint32_t)((1ULL << k) - 1);
        if (f > 1) {
            x1038_divsmall(S, &Slen, f);
            x1038_divsmall(S, &Slen, f);
        }
    }

    for (i = Slen - 1; i >= 0; i--) {
        int b;
        for (b = 3; b >= 0; b--) {
            r *= 256;
            r += (seq_t)((S[i] >> (8 * b)) & 255u);
        }
    }

    for (t = 0; t < g_nt; t++) { free(g_tkey[t]); free(g_tN[t]); }
    free(g_tkey); free(g_tklen); free(g_tN); free(g_tNlen);
    free(S); free(z); free(N2); free(T);
    free(g_G);
    free(g_pp); free(g_plen);
    free(g_ipoly); free(g_iord); free(g_ideg);
    return r;
}
