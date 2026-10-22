/*
 * A004106  Number of line-self-dual nets (or edge-self-dual nets) with n
 *          nodes.
 * https://oeis.org/A004106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n!) * Sum_{s in S_n} 2^c(s) * 3^e(s), where c(s) is
 *            the number of cycles of s on the nodes and e(s) is the number of
 *            even-length cycles that s induces on the n(n-1)/2 node pairs (de
 *            Bruijn count of nets fixed by s composed with edge-sign
 *            reversal). The code sums over the cycle types (pa...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   badd, bitlen, gcdi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/badd.h"
#include "../utils/bitlen.h"
#include "../utils/gcdi.h"

static int g_L, g_np;
static uint32_t *g_S, *g_W;
static int *g_pk, *g_pm;

static void bmul(uint32_t *a, int L, uint32_t x) {
    uint64_t c = 0;
    for (int i = 0; i < L; i++) {
        uint64_t t = (uint64_t)a[i] * x + c;
        a[i] = (uint32_t)t;
        c = t >> 32;
    }
}

static void bdiv(uint32_t *a, int L, uint32_t x) {
    uint64_t r = 0;
    for (int i = L - 1; i >= 0; i--) {
        uint64_t cur = (r << 32) | a[i];
        a[i] = (uint32_t)(cur / x);
        r = cur % x;
    }
}

static void rec(int kmax, int r, long long E, int depth) {
    int L = g_L;
    uint32_t *W = g_W + (size_t)depth * L;
    uint32_t *X = W + L;
    if (r == 0) { badd(g_S + (size_t)E * L, L, W, L); return; }
    if (kmax <= 0) return;
    int k = kmax < r ? kmax : r;
    for (; k >= 2; k--) {
        long long base = 0;
        for (int i = 0; i < g_np; i++)
            if ((k % 2 == 0) || (g_pk[i] % 2 == 0))
                base += (long long)g_pm[i] * gcdi(k, g_pk[i]);
        long long wk = (k % 2 == 0) ? (k - 2) / 2 + (k % 4 == 0) : 0;
        memcpy(X, W, (size_t)L * sizeof(uint32_t));
        int rr = r;
        g_pk[g_np] = k;
        g_np++;
        for (int m = 1; m * k <= r; m++) {
            uint64_t acc = 2;
            for (int j = 0; j < k; j++) {
                uint64_t v = (uint64_t)rr--;
                if (acc * v > 0xFFFFFFFFull) { bmul(X, L, (uint32_t)acc); acc = v; }
                else acc *= v;
            }
            bmul(X, L, (uint32_t)acc);
            bdiv(X, L, (uint32_t)(k * m));
            long long Em = E + (long long)m * (wk + base)
                         + ((k % 2 == 0) ? (long long)m * (m - 1) / 2 * k : 0);
            g_pm[g_np - 1] = m;
            rec(k - 1, r - k * m, Em, depth + 1);
        }
        g_np--;
    }
    {
        long long ev = 0;
        for (int i = 0; i < g_np; i++)
            if (g_pk[i] % 2 == 0) ev += g_pm[i];
        long long E1 = E + (long long)r * ev;
        memcpy(X, W, (size_t)L * sizeof(uint32_t));
        int s = r;
        while (s > 0) {
            int t = s > 31 ? 31 : s;
            bmul(X, L, (uint32_t)1u << t);
            s -= t;
        }
        badd(g_S + (size_t)E1 * L, L, X, L);
    }
}

seq_t a(idx_t n) {
    int nn = (int)n;
    long long Emax = (long long)nn * (nn - 1) / 2;
    if (Emax < 0) Emax = 0;
    long long lf = 0;
    for (int i = 2; i <= nn + 1; i++) lf += bitlen(i);
    int L = (int)((lf + nn + 64 + 2 * bitlen(nn)) / 32 + 2);
    int LT = (int)((Emax * 1585) / 1000 + 1 + lf + nn + 64) / 32 + L + 2;
    g_L = L;
    g_S = (uint32_t *)calloc((size_t)(Emax + 1) * L, sizeof(uint32_t));
    g_W = (uint32_t *)calloc((size_t)(nn + 3) * L, sizeof(uint32_t));
    g_pk = (int *)calloc((size_t)nn + 2, sizeof(int));
    g_pm = (int *)calloc((size_t)nn + 2, sizeof(int));
    g_np = 0;
    g_W[0] = 1;
    rec(nn, nn, 0, 0);

    uint32_t *T = (uint32_t *)calloc((size_t)LT, sizeof(uint32_t));
    for (long long E = Emax; E >= 0; E--) {
        bmul(T, LT, 3);
        badd(T, LT, g_S + (size_t)E * L, L);
    }
    {
        uint64_t acc = 1;
        for (int i = 2; i <= nn; i++) {
            if (acc * (uint64_t)i > 0xFFFFFFFFull) { bdiv(T, LT, (uint32_t)acc); acc = (uint64_t)i; }
            else acc *= (uint64_t)i;
        }
        bdiv(T, LT, (uint32_t)acc);
    }
    seq_t res = 0;
    int top = LT - 1;
    while (top > 0 && T[top] == 0) top--;
    for (int i = top; i >= 0; i--)
        for (int sh = 24; sh >= 0; sh -= 8)
            res = (seq_t)(res * 256 + (seq_t)((T[i] >> sh) & 255u));

    free(T);
    free(g_S);
    free(g_W);
    free(g_pk);
    free(g_pm);
    return res;
}
