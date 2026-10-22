/*
 * A006651  Number of graphs with n nodes, n+2 edges and no isolated vertices.
 * https://oeis.org/A006651
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = G(n,n+2) - G(n-1,n+2), where G(k,e) = number of graphs on
 *            k nodes with e edges (removing an isolated node maps graphs that
 *            have one onto graphs on n-1 nodes). G(k,e) is computed by Polya
 *            enumeration: G(k,e) = Sum_{partitions lambda of k} (1/z_lambda)
 *            * [y^e] Product over edge orbits O of (...
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   bitlen_u64, gcd_ll, seq_is_fixed_width, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/u128_to_seq.h"
#include "../utils/bitlen_u64.h"
#include "../utils/gcd_ll.h"

typedef unsigned __int128 u128_t;

static uint64_t mulm(uint64_t a, uint64_t b, uint64_t p) { return (uint64_t)((u128_t)a * b % p); }

static uint64_t powm(uint64_t a, uint64_t e, uint64_t p)
{
    uint64_t r = 1 % p;
    a %= p;
    while (e) {
        if (e & 1) r = mulm(r, a, p);
        a = mulm(a, a, p);
        e >>= 1;
    }
    return r;
}

static int mr_prime(uint64_t n)
{
    static const uint64_t bases[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    uint64_t d;
    int s = 0, i, r;
    if (n < 2) return 0;
    for (i = 0; i < 12; i++)
        if (n % bases[i] == 0) return n == bases[i];
    d = n - 1;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (i = 0; i < 12; i++) {
        uint64_t x = powm(bases[i], d, n);
        int ok = 0;
        if (x == 1 || x == n - 1) continue;
        for (r = 1; r < s; r++) {
            x = mulm(x, x, n);
            if (x == n - 1) { ok = 1; break; }
        }
        if (!ok) return 0;
    }
    return 1;
}

static uint64_t *primes_c = NULL;
static int nprimes_c = 0;

static void need_primes(int k)
{
    uint64_t c;
    if (nprimes_c >= k) return;
    primes_c = (uint64_t *)realloc(primes_c, (size_t)k * sizeof(uint64_t));
    c = nprimes_c ? primes_c[nprimes_c - 1] - 2 : (((uint64_t)1) << 62) - 1;
    while (nprimes_c < k) {
        if (mr_prime(c)) primes_c[nprimes_c++] = c;
        c -= 2;
    }
}

static uint64_t gP, gacc;
static int gM, gnp;
static uint64_t gF;
static uint64_t *ginv, *gstk, *gbE, *gbc;
static int *gpv, *gpc;

/* q *= (1 + y^L)^c, truncated to degree gM */
static void mulpow(uint64_t *q, long long L, uint64_t c)
{
    int Li, t, j, i, k;
    if (c == 0 || L > gM) return;
    Li = (int)L;
    t = gM / Li;
    if ((u128_t)c * (uint64_t)(gM - Li + 1) <= (u128_t)4 * (uint64_t)gM * (uint64_t)(t + 1)) {
        uint64_t r;
        for (r = 0; r < c; r++)
            for (j = gM; j >= Li; j--) {
                uint64_t v = q[j] + q[j - Li];
                if (v >= gP) v -= gP;
                q[j] = v;
            }
    } else {
        gbc[0] = 1;
        for (i = 1; i <= t; i++)
            gbc[i] = ((uint64_t)i > c) ? 0
                     : mulm(mulm(gbc[i - 1], (c - (uint64_t)i + 1) % gP, gP), ginv[i], gP);
        for (j = gM; j >= Li; j--) {
            uint64_t s = q[j];
            for (i = 1, k = j - Li; i <= t && k >= 0; i++, k -= Li) {
                s += mulm(gbc[i], q[k], gP);
                if (s >= gP) s -= gP;
            }
            q[j] = s;
        }
    }
}

static void dfs(int depth, int rem, int maxp, uint64_t invz)
{
    uint64_t *q = gstk + (size_t)depth * (size_t)(gM + 1);
    int a, i;
    if (rem == 0) {
        uint64_t s = 0;
        int j;
        for (j = 0; j <= gM; j++) {
            s += mulm(q[j], gbE[gM - j], gP);
            if (s >= gP) s -= gP;
        }
        gacc = (gacc + mulm(s, invz, gP)) % gP;
        return;
    }
    for (a = (maxp < rem ? maxp : rem); a >= 2; a--) {
        uint64_t *r;
        int newcnt, pushed;
        if (rem - a == 1) continue;
        r = q + (gM + 1);
        memcpy(r, q, (size_t)(gM + 1) * sizeof(uint64_t));
        if (a & 1) mulpow(r, a, (uint64_t)((a - 1) / 2));
        else { mulpow(r, a, (uint64_t)((a - 2) / 2)); mulpow(r, a / 2, 1); }
        for (i = 0; i < gnp; i++) {
            long long b = gpv[i], g = gcd_ll(a, b);
            mulpow(r, (long long)a / g * b, (uint64_t)g * (uint64_t)gpc[i]);
        }
        mulpow(r, a, gF);
        if (gnp > 0 && gpv[gnp - 1] == a) { newcnt = ++gpc[gnp - 1]; pushed = 0; }
        else { gpv[gnp] = a; gpc[gnp] = 1; gnp++; newcnt = 1; pushed = 1; }
        dfs(depth + 1, rem - a, a, mulm(mulm(invz, ginv[a], gP), ginv[newcnt], gP));
        if (pushed) gnp--; else gpc[gnp - 1]--;
    }
}

/* number of graphs on N nodes with gM edges, mod gP */
static uint64_t count_graphs(int N)
{
    uint64_t total = 0, invf = 1;
    int f, i;
    for (f = 0; f <= N; f++) {
        int R = N - f;
        uint64_t E = (uint64_t)f * (uint64_t)(f > 0 ? f - 1 : 0) / 2;
        if (f > 0) invf = mulm(invf, ginv[f], gP);
        if (R == 1) continue;
        gbE[0] = 1;
        for (i = 1; i <= gM; i++)
            gbE[i] = ((uint64_t)i > E) ? 0
                     : mulm(mulm(gbE[i - 1], (E - (uint64_t)i + 1) % gP, gP), ginv[i], gP);
        memset(gstk, 0, (size_t)(gM + 1) * sizeof(uint64_t));
        gstk[0] = 1;
        gF = (uint64_t)f;
        gnp = 0;
        gacc = 0;
        dfs(0, R, R, invf);
        total = (total + gacc) % gP;
    }
    return total;
}

seq_t a(idx_t n)
{
    int nn = (int)n, M = nn + 2, k, i, j;
    long long bits = 1;
    int bl;
    uint64_t *res, *dig;
    seq_t out;

    bl = bitlen_u64((uint64_t)nn * (uint64_t)(nn - 1) / 2);
    for (i = 1; i <= M; i++) {
        int fl = bitlen_u64((uint64_t)i) - 1;
        bits += bl - fl;
    }
    if (seq_is_fixed_width() && bits > 128) bits = 128;
    k = (int)(bits / 61) + 1;
    need_primes(k);

    gM = M;
    ginv = (uint64_t *)malloc((size_t)(M + 2) * sizeof(uint64_t));
    gbE = (uint64_t *)malloc((size_t)(M + 2) * sizeof(uint64_t));
    gbc = (uint64_t *)malloc((size_t)(M + 2) * sizeof(uint64_t));
    gstk = (uint64_t *)malloc((size_t)(nn / 2 + 3) * (size_t)(M + 1) * sizeof(uint64_t));
    gpv = (int *)malloc((size_t)(nn + 2) * sizeof(int));
    gpc = (int *)malloc((size_t)(nn + 2) * sizeof(int));
    res = (uint64_t *)malloc((size_t)k * sizeof(uint64_t));
    dig = (uint64_t *)malloc((size_t)k * sizeof(uint64_t));

    for (i = 0; i < k; i++) {
        uint64_t g1, g0;
        gP = primes_c[i];
        ginv[1] = 1;
        for (j = 2; j <= M; j++)
            ginv[j] = mulm(gP - gP / (uint64_t)j, ginv[gP % (uint64_t)j], gP);
        g1 = count_graphs(nn);
        g0 = count_graphs(nn - 1);
        res[i] = (g1 + gP - g0) % gP;
    }

    /* Garner mixed-radix digits */
    for (i = 0; i < k; i++) {
        uint64_t p = primes_c[i], x = res[i] % p;
        for (j = 0; j < i; j++) {
            uint64_t pj = primes_c[j] % p;
            x = (x + p - dig[j] % p) % p;
            x = mulm(x, powm(pj, p - 2, p), p);
        }
        dig[i] = x;
    }

    if (seq_is_fixed_width()) {
        u128_t r = 0;
        for (i = k - 1; i >= 0; i--) r = r * primes_c[i] + dig[i];
        out = u128_to_seq(r);
    } else {
        seq_t r = 0;
        for (i = k - 1; i >= 0; i--) {
            idx_t p = (idx_t)primes_c[i], d = (idx_t)dig[i];
            r = r * p + d;
        }
        out = r;
    }

    free(ginv); free(gbE); free(gbc); free(gstk); free(gpv); free(gpc);
    free(res); free(dig);
    return out;
}
