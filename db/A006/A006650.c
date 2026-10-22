/*
 * A006650  Number of graphs with n nodes, n+1 edges and no isolated vertices.
 * https://oeis.org/A006650
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = G(n,n+1) - G(n-1,n+1), where G(k,m) is the number of
 *            graphs on k unlabeled nodes with m edges, i.e. [x^m] of the
 *            Polya sum over partitions lambda of k of (1/z_lambda) * prod
 *            over the edge cycles of lambda of (1 + x^len). Computed by
 *            enumerating partitions mod several 31-bit primes, then CRT
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   a6650_gcd, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/a6650_gcd.h"

typedef uint64_t u64;

static int K6650, M6650;
static long m6650;
static u64 *pr6650 = NULL;
static int npr6650 = 0;
static u64 **inv6650, **ifact6650;
static u64 *poly6650, *scr6650, *bb6650, *wst6650, *acc6650;
static long *dk6650, *dc6650;
static int nd6650;

static int a6650_isp(u64 x)
{
    u64 d;
    if (x % 2 == 0) return 0;
    for (d = 3; d * d <= x; d += 2)
        if (x % d == 0) return 0;
    return 1;
}

static void a6650_primes(int k)
{
    u64 c;
    if (npr6650 >= k) return;
    pr6650 = (u64 *)realloc(pr6650, (size_t)k * sizeof(u64));
    c = npr6650 ? pr6650[npr6650 - 1] - 2 : 2147483647ULL;
    while (npr6650 < k) {
        if (a6650_isp(c)) pr6650[npr6650++] = c;
        c -= 2;
    }
}

static u64 a6650_powmod(u64 b, u64 e, u64 p)
{
    u64 r = 1;
    b %= p;
    while (e) {
        if (e & 1) r = r * b % p;
        b = b * b % p;
        e >>= 1;
    }
    return r;
}

/* Q *= (1 + x^len)^e, truncated at degree m, for every prime */
static void a6650_mulpow(u64 *Q, u64 len, u64 e)
{
    long m = m6650, d;
    u64 maxi, i, t;
    int j;
    if (e == 0 || len > (u64)m) return;
    maxi = (u64)m / len;
    for (j = 0; j < K6650; j++) {
        u64 *q = Q + (size_t)j * M6650, p = pr6650[j];
        if (e <= maxi) {
            for (t = 0; t < e; t++)
                for (d = m; d >= (long)len; d--) {
                    u64 v = q[d] + q[d - len];
                    if (v >= p) v -= p;
                    q[d] = v;
                }
        } else {
            bb6650[0] = 1;
            for (i = 0; i < maxi; i++)
                bb6650[i + 1] = bb6650[i] * ((e - i) % p) % p * inv6650[j][i + 1] % p;
            for (d = m; d >= 0; d--) {
                u64 s = 0;
                for (i = 0; i * len <= (u64)d; i++)
                    s = (s + bb6650[i] * q[d - i * len]) % p;
                q[d] = s;
            }
        }
    }
}

static void a6650_dfs(int depth, long r, long maxp)
{
    u64 *P = poly6650 + (size_t)depth * K6650 * M6650;
    u64 *w = wst6650 + (size_t)depth * K6650;
    long m = m6650, L, i;
    int j, t;
    if (r == 0) {
        for (j = 0; j < K6650; j++)
            acc6650[j] = (acc6650[j] + w[j] * P[(size_t)j * M6650 + m]) % pr6650[j];
        return;
    }
    /* all remaining parts are 1 */
    {
        u64 E = (u64)r * (u64)(r - 1) / 2;
        memcpy(scr6650, P, (size_t)K6650 * M6650 * sizeof(u64));
        for (t = 0; t < nd6650; t++)
            a6650_mulpow(scr6650, (u64)dk6650[t], (u64)r * (u64)dc6650[t]);
        for (j = 0; j < K6650; j++) {
            u64 p = pr6650[j], b = 1, s = 0;
            for (i = 0; i <= m; i++) {
                if ((u64)i > E) break;
                s = (s + b * scr6650[(size_t)j * M6650 + (m - i)]) % p;
                b = b * ((E - (u64)i) % p) % p * inv6650[j][i + 1] % p;
            }
            acc6650[j] = (acc6650[j] + s * w[j] % p * ifact6650[j][r]) % p;
        }
    }
    for (L = (r < maxp ? r : maxp); L >= 2; L--) {
        u64 *NP = poly6650 + (size_t)(depth + 1) * K6650 * M6650;
        u64 *nw = wst6650 + (size_t)(depth + 1) * K6650;
        long cL;
        memcpy(NP, P, (size_t)K6650 * M6650 * sizeof(u64));
        if (L & 1) a6650_mulpow(NP, (u64)L, (u64)((L - 1) / 2));
        else {
            a6650_mulpow(NP, (u64)L, (u64)(L / 2 - 1));
            a6650_mulpow(NP, (u64)(L / 2), 1);
        }
        for (t = 0; t < nd6650; t++) {
            long g = a6650_gcd(L, dk6650[t]);
            a6650_mulpow(NP, (u64)(L / g) * (u64)dk6650[t], (u64)g * (u64)dc6650[t]);
        }
        cL = (nd6650 > 0 && dk6650[nd6650 - 1] == L) ? dc6650[nd6650 - 1] : 0;
        for (j = 0; j < K6650; j++)
            nw[j] = w[j] * inv6650[j][L] % pr6650[j] * inv6650[j][cL + 1] % pr6650[j];
        if (cL > 0) dc6650[nd6650 - 1]++;
        else { dk6650[nd6650] = L; dc6650[nd6650] = 1; nd6650++; }
        a6650_dfs(depth + 1, r - L, L);
        if (cL > 0) dc6650[nd6650 - 1]--;
        else nd6650--;
    }
}

static void a6650_count(long nn, u64 *out)
{
    int j;
    memset(poly6650, 0, (size_t)K6650 * M6650 * sizeof(u64));
    for (j = 0; j < K6650; j++) {
        poly6650[(size_t)j * M6650] = 1;
        wst6650[j] = 1;
        acc6650[j] = 0;
    }
    nd6650 = 0;
    a6650_dfs(0, nn, nn);
    for (j = 0; j < K6650; j++) out[j] = acc6650[j];
}

seq_t a(idx_t n)
{
    long nn = (long)n, levels, i;
    int j, k;
    u64 bits, *g1, *g2, *x;
    seq_t res;

    bits = 2 * (u64)n + 64;
    K6650 = (int)(bits / 30 + 1);
    if (seq_is_fixed_width() && K6650 > 5) K6650 = 5;
    a6650_primes(K6650);
    m6650 = nn + 1;
    M6650 = (int)(m6650 + 1);

    inv6650 = (u64 **)malloc((size_t)K6650 * sizeof(u64 *));
    ifact6650 = (u64 **)malloc((size_t)K6650 * sizeof(u64 *));
    for (j = 0; j < K6650; j++) {
        u64 p = pr6650[j];
        inv6650[j] = (u64 *)malloc((size_t)(nn + 4) * sizeof(u64));
        ifact6650[j] = (u64 *)malloc((size_t)(nn + 4) * sizeof(u64));
        inv6650[j][0] = 0;
        inv6650[j][1] = 1;
        for (i = 2; i < nn + 4; i++)
            inv6650[j][i] = (p - (p / (u64)i) * inv6650[j][p % (u64)i] % p) % p;
        ifact6650[j][0] = 1;
        for (i = 1; i < nn + 4; i++)
            ifact6650[j][i] = ifact6650[j][i - 1] * inv6650[j][i] % p;
    }
    levels = nn / 2 + 3;
    poly6650 = (u64 *)malloc((size_t)levels * K6650 * M6650 * sizeof(u64));
    wst6650 = (u64 *)malloc((size_t)levels * K6650 * sizeof(u64));
    scr6650 = (u64 *)malloc((size_t)K6650 * M6650 * sizeof(u64));
    bb6650 = (u64 *)malloc((size_t)(M6650 + 1) * sizeof(u64));
    acc6650 = (u64 *)malloc((size_t)K6650 * sizeof(u64));
    dk6650 = (long *)malloc((size_t)levels * sizeof(long));
    dc6650 = (long *)malloc((size_t)levels * sizeof(long));
    g1 = (u64 *)malloc((size_t)K6650 * sizeof(u64));
    g2 = (u64 *)malloc((size_t)K6650 * sizeof(u64));
    x = (u64 *)malloc((size_t)K6650 * sizeof(u64));

    a6650_count(nn, g1);
    a6650_count(nn - 1, g2);

    for (j = 0; j < K6650; j++) {
        u64 p = pr6650[j];
        u64 v = (g1[j] + p - g2[j]) % p;
        for (k = 0; k < j; k++) {
            v = (v + p - x[k] % p) % p;
            v = v * a6650_powmod(pr6650[k] % p, p - 2, p) % p;
        }
        x[j] = v;
    }
    res = 0;
    for (j = K6650 - 1; j >= 0; j--)
        res = res * (seq_t)pr6650[j] + (seq_t)x[j];

    for (j = 0; j < K6650; j++) { free(inv6650[j]); free(ifact6650[j]); }
    free(inv6650); free(ifact6650);
    free(poly6650); free(wst6650); free(scr6650); free(bb6650); free(acc6650);
    free(dk6650); free(dc6650); free(g1); free(g2); free(x);
    return res;
}
