/*
 * A002756  Number of bipartite partitions of n white objects and 7 black
 *          ones.
 * https://oeis.org/A002756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. prod_{(i,j)!=(0,0)} 1/(1 - x^i y^j); a(n) = [x^n y^7]. The
 *            y^7 coefficient expands to a(n) = [x^n] P(x) * Sum_{7 = Sum_j
 *            j*t_j} Prod_{j=1..7} 1/((1-x)(1-x^2)...(1-x^{t_j})), summed over
 *            the 15 partitions of 7, where P(x) = Prod_{k>=1} 1/(1-x^k) (the
 *            partition numbers, from Euler's pentagonal...
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 58, u64 n <= 243,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_zero_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_zero_arr.h"

static seq_t *g_T = 0;
static idx_t g_len = 0;

static seq_t *g_P = 0;
static seq_t *g_tmp = 0;
static seq_t *g_acc = 0;
static idx_t g_N = 0;
static int g_t[8];

static void apply_partition(void)
{
    idx_t m, N = g_N;
    int j, k;
    for (m = 0; m <= N; m++) g_tmp[m] = g_P[m];
    for (j = 1; j <= 7; j++) {
        for (k = 1; k <= g_t[j]; k++) {
            for (m = (idx_t)k; m <= N; m++) g_tmp[m] += g_tmp[m - (idx_t)k];
        }
    }
    for (m = 0; m <= N; m++) g_acc[m] += g_tmp[m];
}

static void gen_parts(int j, int r)
{
    int c;
    if (j == 0) {
        if (r == 0) apply_partition();
        return;
    }
    for (c = 0; c * j <= r; c++) {
        g_t[j] = c;
        gen_parts(j - 1, r - c * j);
    }
    g_t[j] = 0;
}

static void build(idx_t N)
{
    idx_t L = N + 1, m;
    int j;
    g_N = N;
    g_P = alloc_zero_arr(L);
    g_tmp = alloc_zero_arr(L);
    g_acc = alloc_zero_arr(L);

    g_P[0] = 1;
    for (m = 1; m <= N; m++) {
        seq_t pos = 0, neg = 0;
        idx_t k;
        for (k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2;
            idx_t g2 = k * (3 * k + 1) / 2;
            if (g1 > m) break;
            if (k & 1) {
                pos += g_P[m - g1];
                if (g2 <= m) pos += g_P[m - g2];
            } else {
                neg += g_P[m - g1];
                if (g2 <= m) neg += g_P[m - g2];
            }
        }
        g_P[m] = pos - neg;
    }

    for (j = 0; j < 8; j++) g_t[j] = 0;
    gen_parts(7, 7);

    if (g_T) free(g_T);
    g_T = g_acc;
    g_len = L;
    free(g_P);
    free(g_tmp);
    g_P = 0;
    g_tmp = 0;
    g_acc = 0;
}

seq_t a(idx_t n)
{
    if (n >= g_len) {
        idx_t N = n;
        if (N < 2 * g_len) N = 2 * g_len;
        if (N < 64) N = 64;
        build(N);
    }
    return g_T[n];
}
