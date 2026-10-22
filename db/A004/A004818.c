/*
 * A004818  Numbers that are the sum of 7 positive 11th powers.
 * https://oeis.org/A004818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 1380, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   p11, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: enumerate all sums of 7 positive 11th powers with bases <= B, sort and dedupe;
   every sum below (B+1)^11 + 6 is then complete. B grows until n terms are known. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/u128_to_seq.h"
#include "../utils/p11.h"

static uint64_t *g_vals = NULL;
static size_t g_nvals = 0;
static int g_B = 0;

static uint64_t g_pw[64];
static uint64_t *g_buf = NULL;
static size_t g_cnt = 0;

static int cmp_u64(const void *x, const void *y) {
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u < v) ? -1 : (u > v) ? 1 : 0;
}

static void gen(int depth, int minb, int B, uint64_t partial) {
    int b;
    if (depth == 7) {
        g_buf[g_cnt++] = partial;
        return;
    }
    for (b = minb; b <= B; b++) gen(depth + 1, b, B, partial + g_pw[b]);
}

static void build(int B) {
    size_t total = 1;
    int i, b;
    uint64_t limit;
    size_t k, m;
    /* C(B+6, 7) multisets */
    for (i = 1; i <= 7; i++) total = total * (size_t)(B + 7 - i) / (size_t)i;
    for (b = 1; b <= B + 1; b++) g_pw[b] = p11((uint64_t)b);
    g_buf = (uint64_t *)malloc(total * sizeof(uint64_t));
    g_cnt = 0;
    gen(0, 1, B, 0);
    qsort(g_buf, g_cnt, sizeof(uint64_t), cmp_u64);
    limit = g_pw[B + 1] + 6;
    m = 0;
    for (k = 0; k < g_cnt; k++) {
        if (g_buf[k] >= limit) break;
        if (m == 0 || g_buf[k] != g_buf[m - 1]) g_buf[m++] = g_buf[k];
    }
    free(g_vals);
    g_vals = g_buf;
    g_buf = NULL;
    g_nvals = m;
    g_B = B;
}

seq_t a(idx_t n) {
    int B = g_B < 2 ? 2 : g_B;
    if (g_vals == NULL) build(B);
    while (g_nvals < n && B < 40) {
        B++;
        build(B);
    }
    if (n < 1 || n > g_nvals) return u128_to_seq(0);
    return u128_to_seq((unsigned __int128)g_vals[n - 1]);
}
