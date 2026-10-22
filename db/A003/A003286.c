/*
 * A003286  Number of semi-regular digraphs (with loops) on n unlabeled nodes
 *          with each node having out-degree 2.
 * https://oeis.org/A003286
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, nice
 * Formula:   Burnside's lemma over S_n. a(n) = (1/n!) * Sum_{partitions
 *            lambda of n with c_k parts equal to k} n!/Prod_k(k^c_k * c_k!) *
 *            Prod_k f(k)^c_k, where f(k) = binomial(F_k, 2) + T_k is the
 *            number of 2-element out-sets (loops allowed) fixed by sigma^k,
 *            F_k = Sum_{m|k} m*c_m, T_k = Sum_{m|2k, m not divi...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   a3286_add, a3286_div, a3286_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a3286_add.h"
#include "../utils/a3286_div.h"
#include "../utils/a3286_mul.h"

static size_t g_L;
static uint32_t *g_tot;
static size_t g_totlen;
static uint32_t *g_W;
static size_t *g_Wlen;
static uint32_t *g_tmp;
static idx_t *g_pk;
static idx_t *g_pc;

static void a3286_leaf(int d)
{
    uint32_t *W = g_W + (size_t)d * g_L;
    size_t len = g_Wlen[d];
    int i, j;
    memcpy(g_tmp, W, len * sizeof(uint32_t));
    for (i = 0; i < d; i++) {
        idx_t k = g_pk[i];
        uint64_t F = 0, T = 0, f, acc = 1, c;
        for (j = 0; j < d; j++) {
            idx_t p = g_pk[j];
            if (k % p == 0) F += (uint64_t)p * g_pc[j];
            else if ((2 * k) % p == 0) T += (uint64_t)(p / 2) * g_pc[j];
        }
        f = F * (F - 1) / 2 + T;
        if (f == 0) return;
        for (c = 0; c < g_pc[i]; c++) {
            if (acc * f >= ((uint64_t)1 << 32)) {
                a3286_mul(g_tmp, &len, (uint32_t)acc);
                acc = 1;
            }
            acc *= f;
        }
        if (acc > 1) a3286_mul(g_tmp, &len, (uint32_t)acc);
    }
    a3286_add(g_tot, &g_totlen, g_tmp, len);
}

static void a3286_rec(idx_t rem, idx_t maxk, int depth)
{
    idx_t k;
    if (rem == 0) {
        a3286_leaf(depth);
        return;
    }
    k = rem < maxk ? rem : maxk;
    for (; k >= 1; k--) {
        uint32_t *src = g_W + (size_t)depth * g_L;
        uint32_t *dst = g_W + (size_t)(depth + 1) * g_L;
        size_t dlen = g_Wlen[depth];
        idx_t r = rem, c;
        memcpy(dst, src, dlen * sizeof(uint32_t));
        for (c = 1; c * k <= rem; c++) {
            uint64_t acc = 1;
            idx_t t;
            for (t = 0; t < k; t++) {
                uint64_t v = r - t;
                if (acc * v >= ((uint64_t)1 << 32)) {
                    a3286_mul(dst, &dlen, (uint32_t)acc);
                    acc = 1;
                }
                acc *= v;
            }
            if (acc > 1) a3286_mul(dst, &dlen, (uint32_t)acc);
            if (k > 1) a3286_div(dst, &dlen, (uint32_t)k);
            if (c > 1) a3286_div(dst, &dlen, (uint32_t)c);
            r -= k;
            g_Wlen[depth + 1] = dlen;
            g_pk[depth] = k;
            g_pc[depth] = c;
            if (k > 1 || c == rem) a3286_rec(rem - c * k, k - 1, depth + 1);
        }
    }
}

seq_t a(idx_t n)
{
    idx_t bl = 0, t = n, i;
    size_t D, bits;
    seq_t r = 0;
    while (t) { bl++; t >>= 1; }
    bits = (size_t)(3 * n * bl + 64);
    g_L = bits / 32 + 4;
    D = (size_t)n + 2;
    g_tot = (uint32_t *)calloc(g_L, sizeof(uint32_t));
    g_tmp = (uint32_t *)calloc(g_L, sizeof(uint32_t));
    g_W = (uint32_t *)calloc(g_L * (D + 1), sizeof(uint32_t));
    g_Wlen = (size_t *)calloc(D + 1, sizeof(size_t));
    g_pk = (idx_t *)calloc(D, sizeof(idx_t));
    g_pc = (idx_t *)calloc(D, sizeof(idx_t));
    g_totlen = 0;
    g_W[0] = 1;
    g_Wlen[0] = 1;
    a3286_rec(n, n, 0);
    for (i = 2; i <= n; i++) a3286_div(g_tot, &g_totlen, (uint32_t)i);
    {
        size_t j = g_totlen;
        while (j > 0) {
            uint32_t limb;
            j--;
            limb = g_tot[j];
            r = r * (seq_t)65536 + (seq_t)(limb >> 16);
            r = r * (seq_t)65536 + (seq_t)(limb & 0xFFFFu);
        }
    }
    free(g_tot); free(g_tmp); free(g_W); free(g_Wlen); free(g_pk); free(g_pc);
    return r;
}
