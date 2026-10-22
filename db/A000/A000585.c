/*
 * A000585  Number of equivalence classes of Boolean functions of n variables
 *          under GL(n,2).
 * https://oeis.org/A000585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   Burnside's lemma over the conjugacy classes of GL(n,2): a(n) =
 *            (1/|GL(n,2)|) * Sum_{classes C} |C| * 2^cyc(C), where |GL(n,2)|
 *            = 2^(n(n-1)/2) * Prod_{k=1..n} (2^k-1), the classes are the
 *            rational canonical forms (irreducible f != x -> partition
 *            lambda_f with Sum deg(f)*|lambda_f| = n), |C| = |G|...
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   bn_div32, bn_mul32, deg64, gcd64u, irreducible, pgcd, pmulmod,
 *            porder, ppow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/bn_div32.h"
#include "../utils/bn_mul32.h"
#include "../utils/deg64.h"
#include "../utils/gcd64u.h"
#include "../utils/irreducible.h"
#include "../utils/pgcd.h"
#include "../utils/pmulmod.h"
#include "../utils/porder.h"
#include "../utils/ppow.h"

#define MAPCAP 65536

static int g_np, g_e2, g_depth, g_pbtop;
static uint64_t *g_po;
static int *g_pd;
static uint32_t *g_acc, *g_go, *g_tmp;
static size_t g_accl, g_gol;
static int g_cp[256], g_cs[256], g_cl[256], g_pb[256];
static uint64_t g_mL[MAPCAP], g_mC[MAPCAP], g_nL[MAPCAP], g_nC[MAPCAP];

static void add_shift(uint32_t *x, size_t xl, uint64_t s)
{
    size_t w = (size_t)(s / 32), i;
    int b = (int)(s % 32);
    uint64_t carry = 0;
    for (i = 0; i < xl; i++) {
        uint64_t cur = (uint64_t)x[i] << b;
        uint64_t sum = (uint64_t)g_acc[w + i] + (uint32_t)cur + carry;
        g_acc[w + i] = (uint32_t)sum;
        carry = (sum >> 32) + (cur >> 32);
    }
    i = w + xl;
    while (carry && i < g_accl) {
        uint64_t sum = (uint64_t)g_acc[i] + carry;
        g_acc[i] = (uint32_t)sum;
        carry = sum >> 32;
        i++;
    }
}

static void leaf(void)
{
    int K = 1, c, t2 = 0, k;
    uint64_t cyc = 0;
    g_mL[0] = 1; g_mC[0] = 1;
    memcpy(g_tmp, g_go, g_gol * sizeof(uint32_t));
    for (c = 0; c < g_depth; c++) {
        int i = g_cp[c], d = g_pd[i], len = g_cl[c], mx, j, sumsq = 0, a = 0, r;
        int *lam = g_pb + g_cs[c];
        uint64_t prevN = 0;
        int NK = 0;
        mx = lam[0];
        for (j = 1; j <= mx; j++) {
            int cnt = 0;
            for (r = 0; r < len; r++) if (lam[r] >= j) cnt++;
            sumsq += cnt * cnt;
        }
        r = 0;
        while (r < len) {
            int m = 1;
            while (r + m < len && lam[r + m] == lam[r]) m++;
            a += m * (m + 1) / 2;
            for (k = 1; k <= m; k++)
                bn_div32(g_tmp, g_gol, (uint32_t)((1ULL << (d * k)) - 1));
            r += m;
        }
        t2 += d * (sumsq - a);
        for (j = 0; j <= mx; j++) {
            int s = 0, t = 0;
            uint64_t Nj, ce, ln;
            for (r = 0; r < len; r++) s += lam[r] < j ? lam[r] : j;
            Nj = 1ULL << (d * s);
            ce = Nj - prevN;
            prevN = Nj;
            while ((1 << t) < j) t++;
            ln = (j == 0) ? 1 : (g_po[i] << t);
            for (k = 0; k < K; k++) {
                uint64_t L2 = g_mL[k] / gcd64u(g_mL[k], ln) * ln;
                uint64_t C2 = g_mC[k] * ce;
                int q;
                for (q = 0; q < NK; q++) if (g_nL[q] == L2) break;
                if (q == NK) { g_nL[NK] = L2; g_nC[NK] = 0; NK++; }
                g_nC[q] += C2;
            }
        }
        memcpy(g_mL, g_nL, NK * sizeof(uint64_t));
        memcpy(g_mC, g_nC, NK * sizeof(uint64_t));
        K = NK;
    }
    for (k = 0; k < K; k++) cyc += g_mC[k] / g_mL[k];
    add_shift(g_tmp, g_gol, (uint64_t)(g_e2 - t2) + cyc);
}

static void gen(int istart, int rem);

static void part_rec(int i, int left, int maxp, int rem_after, int start)
{
    int p;
    if (left == 0) {
        g_cp[g_depth] = i; g_cs[g_depth] = start; g_cl[g_depth] = g_pbtop - start;
        g_depth++;
        gen(i + 1, rem_after);
        g_depth--;
        return;
    }
    for (p = left < maxp ? left : maxp; p >= 1; p--) {
        g_pb[g_pbtop++] = p;
        part_rec(i, left - p, p, rem_after, start);
        g_pbtop--;
    }
}

static void gen(int istart, int rem)
{
    int i, m;
    if (rem == 0) { leaf(); return; }
    for (i = istart; i < g_np && g_pd[i] <= rem; i++) {
        int d = g_pd[i];
        for (m = 1; d * m <= rem; m++) part_rec(i, m, m, rem - d * m, g_pbtop);
    }
}

seq_t a(idx_t n)
{
    int N = (int)n, d, k;
    size_t cap = 16, i, bits, top;
    uint64_t *pf;
    seq_t r = 0;

    g_np = 0;
    pf = (uint64_t *)malloc(cap * sizeof(uint64_t));
    g_po = (uint64_t *)malloc(cap * sizeof(uint64_t));
    g_pd = (int *)malloc(cap * sizeof(int));
    for (d = 1; d <= N; d++) {
        uint64_t f, xr;
        for (f = (1ULL << d) + 1; f < (1ULL << (d + 1)); f += 2) {
            xr = (d == 1) ? (2 ^ f) : 2;
            if (!irreducible(f, d, xr)) continue;
            if ((size_t)g_np == cap) {
                cap *= 2;
                pf = (uint64_t *)realloc(pf, cap * sizeof(uint64_t));
                g_po = (uint64_t *)realloc(g_po, cap * sizeof(uint64_t));
                g_pd = (int *)realloc(g_pd, cap * sizeof(int));
            }
            pf[g_np] = f; g_pd[g_np] = d; g_po[g_np] = porder(f, d, xr);
            g_np++;
        }
    }

    g_e2 = N * (N - 1) / 2;
    g_gol = (size_t)(N * (N + 1) / 2) / 32 + 2;
    g_go = (uint32_t *)calloc(g_gol, sizeof(uint32_t));
    g_tmp = (uint32_t *)calloc(g_gol, sizeof(uint32_t));
    g_go[0] = 1;
    for (k = 1; k <= N; k++) bn_mul32(g_go, g_gol, (uint32_t)((1ULL << k) - 1));

    bits = ((size_t)1 << N) + (size_t)g_e2 + g_gol * 32 + 64;
    g_accl = bits / 32 + 2;
    g_acc = (uint32_t *)calloc(g_accl, sizeof(uint32_t));

    g_depth = 0; g_pbtop = 0;
    gen(0, N);

    {
        size_t w = (size_t)g_e2 / 32;
        int b = g_e2 % 32;
        for (i = 0; i < g_accl; i++) {
            uint64_t lo = (i + w < g_accl) ? g_acc[i + w] : 0;
            uint64_t hi = (i + w + 1 < g_accl) ? g_acc[i + w + 1] : 0;
            g_acc[i] = (uint32_t)(((hi << 32) | lo) >> b);
        }
    }
    for (k = 1; k <= N; k++) bn_div32(g_acc, g_accl, (uint32_t)((1ULL << k) - 1));

    top = g_accl;
    while (top > 0 && g_acc[top - 1] == 0) top--;
    for (i = top; i-- > 0;) {
        int sh;
        for (sh = 24; sh >= 0; sh -= 8) {
            r *= 256;
            r += (int)((g_acc[i] >> sh) & 255);
        }
    }

    free(pf); free(g_po); free(g_pd);
    free(g_go); free(g_tmp); free(g_acc);
    return r;
}
