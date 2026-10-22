/*
 * A002758  Number of bipartite partitions of n white objects and 9 black
 *          ones.
 * https://oeis.org/A002758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n y^9] Product_{(i,j) != (0,0)} 1/(1 - x^i y^j). Using
 *            Product_{i>=0} 1/(1 - z x^i) = Sum_k z^k / ((1-x)...(1-x^k)):
 *            a(n) = Sum_{t=0..n} S(t)*A000041(n-t), where S(x) = Sum over
 *            partitions of 9 with k_j parts equal to j of Product_j
 *            1/((1-x)(1-x^2)...(1-x^{k_j}))
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 50, u64 n <= 219, u128 n <= 950,
 *            big any n
 * Helpers:   a001028_z
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a001028_z.h"

static idx_t g_cap = 0;
static seq_t *g_S = 0, *g_P = 0, *g_T = 0;
static int g_km[10];

static void add_term(idx_t N)
{
    idx_t i;
    int j, m;
    for (i = 0; i < N; i++) g_T[i] = 0;
    g_T[0] = 1;
    for (j = 1; j <= 9; j++)
        for (m = 1; m <= g_km[j]; m++)
            for (i = (idx_t)m; i < N; i++)
                g_T[i] += g_T[i - (idx_t)m];
    for (i = 0; i < N; i++) g_S[i] += g_T[i];
}

static void gen_parts(int rem, int maxp, idx_t N)
{
    int p;
    if (rem == 0) { add_term(N); return; }
    for (p = (rem < maxp ? rem : maxp); p >= 1; p--) {
        g_km[p]++;
        gen_parts(rem - p, p, N);
        g_km[p]--;
    }
}

static void build(idx_t N)
{
    idx_t i, k;
    int j;
    g_S = a001028_z(N);
    g_P = a001028_z(N);
    g_T = a001028_z(N);
    for (j = 0; j < 10; j++) g_km[j] = 0;
    gen_parts(9, 9, N);
    g_P[0] = 1;
    for (i = 1; i < N; i++) {
        seq_t pos = 0, neg = 0;
        for (k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2;
            idx_t g2 = k * (3 * k + 1) / 2;
            seq_t term;
            if (g1 > i) break;
            term = g_P[i - g1];
            if (g2 <= i) term += g_P[i - g2];
            if (k & 1) pos += term; else neg += term;
        }
        g_P[i] = pos - neg;
    }
    g_cap = N;
}

seq_t a(idx_t n)
{
    seq_t r = 0;
    idx_t t;
    if (n >= g_cap) {
        idx_t N = n + 1;
        if (N < 2 * g_cap) N = 2 * g_cap;
        if (N < 64) N = 64;
        build(N);
    }
    for (t = 0; t <= n; t++) r += g_S[t] * g_P[n - t];
    return r;
}
