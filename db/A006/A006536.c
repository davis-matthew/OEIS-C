/*
 * A006536  Switching classes of digraphs.
 * https://oeis.org/A006536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = number of switching classes of digraphs on N = n+1 nodes.
 *            By Burnside over S_N x (switchings), every permutation admits
 *            exactly 2 valid cycle-parity patterns, which gives a(n) =
 *            Sum_{partitions L of n+1} 2^e(L) / z(L), e(L) = 1 + Sum_i (L_i -
 *            2) + 2*Sum_{i<j} gcd(L_i, L_j), z(L) = Prod_k k...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   mulw, oddi, pw2, seq_is_fixed_width, v2i
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pw2.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/mulw.h"
#include "../utils/oddi.h"
#include "../utils/v2i.h"

static int g_N;
static int *g_gcd;
static int *g_parts;
static int g_cnt;
static bool g_fixed;
static int g_w;
static seq_t g_S;
static seq_t g_O;

static seq_t inv_odd(seq_t o) {
    seq_t x = o;
    int i;
    for (i = 0; i < 8; i++) {
        seq_t t = (seq_t)(2u - mulw(o, x));
        x = mulw(x, t);
    }
    return x;
}

static int gcdi(int x, int y) {
    while (y) { int t = x % y; x = y; y = t; }
    return x;
}

static void rec(int rem, int maxp, long long e, long long t, seq_t o,
                int last, int mult) {
    if (rem == 0) {
        long long s = e + 1 - t;
        if (g_fixed) {
            if (s < g_w) {
                seq_t p = (seq_t)1;
                p = (seq_t)(p << (int)s);
                g_S = (seq_t)(g_S + mulw(p, inv_odd(o)));
            }
        } else {
            g_S += pw2((idx_t)s) * (g_O / o);
        }
        return;
    }
    int k;
    for (k = (rem < maxp ? rem : maxp); k >= 1; k--) {
        long long ne = e + (k - 2);
        int i;
        for (i = 0; i < g_cnt; i++)
            ne += 2 * g_gcd[g_parts[i] * (g_N + 1) + k];
        int nm = (k == last) ? mult + 1 : 1;
        long long nt = t + v2i(k) + v2i(nm);
        seq_t no = mulw(o, (seq_t)oddi(k));
        no = mulw(no, (seq_t)oddi(nm));
        g_parts[g_cnt++] = k;
        rec(rem - k, k, ne, nt, no, k, nm);
        g_cnt--;
    }
}

seq_t a(idx_t n) {
    int N = (int)n + 1;
    int i, j;
    g_N = N;
    g_gcd = (int *)malloc(sizeof(int) * (size_t)(N + 1) * (size_t)(N + 1));
    for (i = 0; i <= N; i++)
        for (j = 0; j <= N; j++)
            g_gcd[i * (N + 1) + j] = gcdi(i, j);
    g_parts = (int *)malloc(sizeof(int) * (size_t)(N + 1));
    g_cnt = 0;
    g_fixed = seq_is_fixed_width();
    if (g_fixed) {
        seq_t x = 0;
        x = (seq_t)(x - 1);
        g_w = 0;
        while (x != 0) { x = (seq_t)(x >> 1); g_w++; }
    }
    g_O = 1;
    if (!g_fixed) {
        for (i = 1; i <= N; i++) g_O = g_O * oddi(i);
    }
    g_S = 0;
    rec(N, N, 0, 0, (seq_t)1, 0, 0);
    free(g_gcd);
    free(g_parts);
    if (g_fixed) return g_S;
    return g_S / g_O;
}
