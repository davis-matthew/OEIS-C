/*
 * A002200  Primes of the form 2^q*3^r*5^s + 1.
 * https://oeis.org/A002200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- a(n) is the n-th prime N with N-1 = 2^q*3^r*5^s. The
 *            code goes through the 5-smooth numbers h one bit-length at a
 *            time. For each h it does trial division of h+1 using residue
 *            tables, then proves primality with the Lucas test, since N-1 = h
 *            is fully factored
 * Range:     u8 n <= 22, u16 n <= 87, u32 n <= 355, u64 n <= 1398,
 *            u128 n <= 5653, big any n
 * Helpers:   a2200_bitlen, a2200_smallprime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/a2200_bitlen.h"
#include "../utils/a2200_smallprime.h"

#define A2200_MAXK 72
typedef unsigned __int128 a2200_u128;

static unsigned a2200_sp[400];
static int a2200_nsp = 0;

static int a2200_K;
static uint64_t a2200_N[A2200_MAXK], a2200_ninv, a2200_one[A2200_MAXK], a2200_r2[A2200_MAXK];

static int a2200_geq(const uint64_t *x, const uint64_t *y)
{
    for (int i = a2200_K - 1; i >= 0; i--)
        if (x[i] != y[i]) return x[i] > y[i];
    return 1;
}

static int a2200_eq(const uint64_t *x, const uint64_t *y)
{
    for (int i = 0; i < a2200_K; i++)
        if (x[i] != y[i]) return 0;
    return 1;
}

static void a2200_subn(uint64_t *x, const uint64_t *y)
{
    uint64_t br = 0;
    for (int i = 0; i < a2200_K; i++) {
        uint64_t t = x[i] - y[i];
        uint64_t b1 = x[i] < y[i];
        uint64_t d = t - br;
        uint64_t b2 = t < br;
        x[i] = d;
        br = b1 | b2;
    }
}

static void a2200_mm(uint64_t *out, const uint64_t *a, const uint64_t *b)
{
    int K = a2200_K;
    uint64_t t[A2200_MAXK + 2];
    memset(t, 0, sizeof(uint64_t) * (size_t)(K + 2));
    for (int i = 0; i < K; i++) {
        a2200_u128 cur, carry = 0;
        for (int j = 0; j < K; j++) {
            cur = (a2200_u128)a[j] * b[i] + t[j] + carry;
            t[j] = (uint64_t)cur;
            carry = cur >> 64;
        }
        cur = (a2200_u128)t[K] + carry;
        t[K] = (uint64_t)cur;
        t[K + 1] = (uint64_t)(cur >> 64);
        uint64_t m = t[0] * a2200_ninv;
        cur = (a2200_u128)m * a2200_N[0] + t[0];
        carry = cur >> 64;
        for (int j = 1; j < K; j++) {
            cur = (a2200_u128)m * a2200_N[j] + t[j] + carry;
            t[j - 1] = (uint64_t)cur;
            carry = cur >> 64;
        }
        cur = (a2200_u128)t[K] + carry;
        t[K - 1] = (uint64_t)cur;
        t[K] = t[K + 1] + (uint64_t)(cur >> 64);
    }
    if (t[K] || a2200_geq(t, a2200_N)) a2200_subn(t, a2200_N);
    memcpy(out, t, sizeof(uint64_t) * (size_t)K);
}

static void a2200_powh(uint64_t *y, const uint64_t *am, int q, int r, int s)
{
    uint64_t t[A2200_MAXK];
    memcpy(y, am, sizeof(uint64_t) * (size_t)a2200_K);
    for (int i = 0; i < q; i++) a2200_mm(y, y, y);
    for (int i = 0; i < r; i++) { a2200_mm(t, y, y); a2200_mm(y, t, y); }
    for (int i = 0; i < s; i++) { a2200_mm(t, y, y); a2200_mm(t, t, t); a2200_mm(y, t, y); }
}

/* N = 2^q 3^r 5^s + 1 (q >= 1, N > 2^21), k limbs */
static bool a2200_lucas(const uint64_t *N, int k, int q, int r, int s)
{
    a2200_K = k;
    memcpy(a2200_N, N, sizeof(uint64_t) * (size_t)k);
    uint64_t inv = N[0];
    for (int i = 0; i < 6; i++) inv *= 2 - N[0] * inv;
    a2200_ninv = (uint64_t)0 - inv;

    uint64_t x[A2200_MAXK];
    memset(x, 0, sizeof(x));
    x[0] = 1;
    for (int i = 0; i < 128 * k; i++) {
        uint64_t c = x[k - 1] >> 63;
        for (int j = k - 1; j > 0; j--) x[j] = (x[j] << 1) | (x[j - 1] >> 63);
        x[0] <<= 1;
        if (c || a2200_geq(x, a2200_N)) a2200_subn(x, a2200_N);
    }
    memcpy(a2200_r2, x, sizeof(uint64_t) * (size_t)k);
    memset(x, 0, sizeof(x));
    x[0] = 1;
    a2200_mm(a2200_one, x, a2200_r2);

    bool n2 = true, n3 = r > 0, n5 = s > 0;
    uint64_t am[A2200_MAXK], y[A2200_MAXK], f[A2200_MAXK];
    for (uint64_t a = 2; a < 3000; a++) {
        memset(x, 0, sizeof(x));
        x[0] = a;
        a2200_mm(am, x, a2200_r2);
        a2200_powh(y, am, q - 1, r, s);
        a2200_mm(f, y, y);
        if (!a2200_eq(f, a2200_one)) return false;
        if (n2 && !a2200_eq(y, a2200_one)) n2 = false;
        if (n3) { a2200_powh(y, am, q, r - 1, s); if (!a2200_eq(y, a2200_one)) n3 = false; }
        if (n5) { a2200_powh(y, am, q, r, s - 1); if (!a2200_eq(y, a2200_one)) n5 = false; }
        if (!n2 && !n3 && !n5) return true;
    }
    return false;
}

static void a2200_mulsmall(uint64_t *x, int w, uint64_t m)
{
    a2200_u128 c = 0;
    for (int i = 0; i < w; i++) {
        c += (a2200_u128)x[i] * m;
        x[i] = (uint64_t)c;
        c >>= 64;
    }
}

/* odd parts m = 3^r 5^s with bit length <= pcap */
static int a2200_pcap = 0, a2200_np = 0, a2200_pc = 0;
static int *a2200_pr = NULL, *a2200_ps = NULL, *a2200_pbl = NULL;
static uint16_t *a2200_pres = NULL, *a2200_p2 = NULL;

static void a2200_rebuild(int cap)
{
    int nsp = a2200_nsp;
    int W = cap / 64 + 2;
    uint64_t m3[A2200_MAXK], m[A2200_MAXK];
    uint16_t r3[400], rs[400];
    a2200_np = 0;
    memset(m3, 0, sizeof(m3));
    m3[0] = 1;
    for (int i = 0; i < nsp; i++) r3[i] = 1;
    for (int r = 0;; r++) {
        if (a2200_bitlen(m3, W) > cap) break;
        memcpy(m, m3, sizeof(m));
        memcpy(rs, r3, sizeof(rs));
        for (int s = 0;; s++) {
            int bl = a2200_bitlen(m, W);
            if (bl > cap) break;
            if (a2200_np == a2200_pc) {
                a2200_pc = a2200_pc ? a2200_pc * 2 : 1024;
                a2200_pr = (int *)realloc(a2200_pr, sizeof(int) * (size_t)a2200_pc);
                a2200_ps = (int *)realloc(a2200_ps, sizeof(int) * (size_t)a2200_pc);
                a2200_pbl = (int *)realloc(a2200_pbl, sizeof(int) * (size_t)a2200_pc);
                a2200_pres = (uint16_t *)realloc(a2200_pres, sizeof(uint16_t) * (size_t)a2200_pc * (size_t)nsp);
            }
            a2200_pr[a2200_np] = r;
            a2200_ps[a2200_np] = s;
            a2200_pbl[a2200_np] = bl;
            memcpy(a2200_pres + (size_t)a2200_np * (size_t)nsp, rs, sizeof(uint16_t) * (size_t)nsp);
            a2200_np++;
            a2200_mulsmall(m, W, 5);
            for (int i = 0; i < nsp; i++) rs[i] = (uint16_t)((rs[i] * 5u) % a2200_sp[i]);
        }
        a2200_mulsmall(m3, W, 3);
        for (int i = 0; i < nsp; i++) r3[i] = (uint16_t)((r3[i] * 3u) % a2200_sp[i]);
    }
    free(a2200_p2);
    a2200_p2 = (uint16_t *)malloc(sizeof(uint16_t) * (size_t)(cap + 2) * (size_t)nsp);
    for (int i = 0; i < nsp; i++) a2200_p2[i] = 1;
    for (int q = 1; q <= cap + 1; q++)
        for (int i = 0; i < nsp; i++)
            a2200_p2[(size_t)q * nsp + i] = (uint16_t)((a2200_p2[(size_t)(q - 1) * nsp + i] * 2u) % a2200_sp[i]);
    a2200_pcap = cap;
}

static int *a2200_fq = NULL, *a2200_fr = NULL, *a2200_fs = NULL;
static idx_t a2200_fcnt = 0, a2200_fcap = 0;
static int a2200_curL = 0;

static void a2200_push(int q, int r, int s)
{
    if (a2200_fcnt == a2200_fcap) {
        a2200_fcap = a2200_fcap ? a2200_fcap * 2 : 1024;
        a2200_fq = (int *)realloc(a2200_fq, sizeof(int) * (size_t)a2200_fcap);
        a2200_fr = (int *)realloc(a2200_fr, sizeof(int) * (size_t)a2200_fcap);
        a2200_fs = (int *)realloc(a2200_fs, sizeof(int) * (size_t)a2200_fcap);
    }
    a2200_fq[a2200_fcnt] = q;
    a2200_fr[a2200_fcnt] = r;
    a2200_fs[a2200_fcnt] = s;
    a2200_fcnt++;
}

static int a2200_sk;
static int a2200_cmp(const void *x, const void *y)
{
    const uint64_t *u = (const uint64_t *)x, *v = (const uint64_t *)y;
    for (int i = a2200_sk - 1; i >= 0; i--)
        if (u[i] != v[i]) return u[i] < v[i] ? -1 : 1;
    return 0;
}

static uint64_t *a2200_hits = NULL;
static size_t a2200_hcap = 0;

/* all N = h+1 with h even 5-smooth and 2^(L-1) <= h < 2^L */
static void a2200_layer(int L)
{
    if (L - 1 > a2200_pcap) a2200_rebuild(L - 1 + 64);
    int k = (L + 63) / 64;
    int rec = k + 3;
    size_t nh = 0;
    int nsp = a2200_nsp;
    for (int pi = 0; pi < a2200_np; pi++) {
        int bl = a2200_pbl[pi];
        if (bl > L - 1) continue;
        int q = L - bl, r = a2200_pr[pi], s = a2200_ps[pi];
        uint64_t x[A2200_MAXK];
        bool ok;
        memset(x, 0, sizeof(x));
        if (L <= 21) {
            uint64_t h = 1;
            for (int i = 0; i < r; i++) h *= 3;
            for (int i = 0; i < s; i++) h *= 5;
            h <<= q;
            x[0] = h + 1;
            ok = a2200_smallprime(h + 1);
        } else {
            const uint16_t *rs = a2200_pres + (size_t)pi * nsp;
            const uint16_t *tw = a2200_p2 + (size_t)q * nsp;
            ok = true;
            for (int i = 0; i < nsp; i++)
                if (((uint32_t)tw[i] * rs[i] + 1) % a2200_sp[i] == 0) { ok = false; break; }
            if (ok) {
                x[0] = 1;
                for (int i = 0; i < r; i++) a2200_mulsmall(x, k, 3);
                for (int i = 0; i < s; i++) a2200_mulsmall(x, k, 5);
                int ws = q / 64, bs = q % 64;
                for (int i = k - 1; i >= 0; i--) {
                    uint64_t v = 0;
                    if (i - ws >= 0) v = x[i - ws] << bs;
                    if (bs && i - ws - 1 >= 0) v |= x[i - ws - 1] >> (64 - bs);
                    x[i] = v;
                }
                x[0] |= 1;
                ok = a2200_lucas(x, k, q, r, s);
            }
        }
        if (ok) {
            if ((nh + 1) * (size_t)rec > a2200_hcap) {
                a2200_hcap = a2200_hcap ? a2200_hcap * 2 : 4096;
                while ((nh + 1) * (size_t)rec > a2200_hcap) a2200_hcap *= 2;
                a2200_hits = (uint64_t *)realloc(a2200_hits, sizeof(uint64_t) * a2200_hcap);
            }
            uint64_t *h = a2200_hits + nh * (size_t)rec;
            memcpy(h, x, sizeof(uint64_t) * (size_t)k);
            h[k] = (uint64_t)q;
            h[k + 1] = (uint64_t)r;
            h[k + 2] = (uint64_t)s;
            nh++;
        }
    }
    a2200_sk = k;
    if (nh > 1) qsort(a2200_hits, nh, sizeof(uint64_t) * (size_t)rec, a2200_cmp);
    for (size_t i = 0; i < nh; i++) {
        uint64_t *h = a2200_hits + i * (size_t)rec;
        a2200_push((int)h[k], (int)h[k + 1], (int)h[k + 2]);
    }
}

static void a2200_init(void)
{
    bool comp[2000];
    memset(comp, 0, sizeof(comp));
    for (int i = 2; i < 2000; i++) {
        if (comp[i]) continue;
        for (int j = i * i; j < 2000; j += i) comp[j] = true;
        if (i > 2) a2200_sp[a2200_nsp++] = (unsigned)i;
    }
    a2200_push(0, 0, 0); /* 2 = 1 + 1 */
    a2200_curL = 2;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (a2200_curL == 0) a2200_init();
    while (a2200_fcnt < n) a2200_layer(a2200_curL++);
    int q = a2200_fq[n - 1], r = a2200_fr[n - 1], s = a2200_fs[n - 1];
    seq_t v = 1;
    for (int i = 0; i < r; i++) v *= 3;
    for (int i = 0; i < s; i++) v *= 5;
    for (int i = 0; i < q; i++) v *= 2;
    v += 1;
    return v;
}
