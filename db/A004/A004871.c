/*
 * A004871  Numbers that are the sum of at most 9 positive 7th powers.
 * https://oeis.org/A004871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- enumerate all multisets of at most 9 positive 7th
 *            powers with sum <= X, sort and dedupe the sums, and double X
 *            until there are at least n terms
 * Range:     u8 n <= 19, u16 n <= 589, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *cache = NULL;
static idx_t cache_len = 0;

static uint64_t g_lim;
static uint64_t g_pw[80];
static int g_maxb;
static uint64_t *g_buf;
static size_t g_cnt, g_cap;

static void push_val(uint64_t s) {
    if (g_cnt == g_cap) {
        g_cap = g_cap ? g_cap * 2 : 1024;
        g_buf = (uint64_t *)realloc(g_buf, g_cap * sizeof(uint64_t));
    }
    g_buf[g_cnt++] = s;
}

static void gen(int k, int maxb, uint64_t s) {
    int b;
    push_val(s);
    if (k == 0) return;
    for (b = 1; b <= maxb; b++) {
        uint64_t t = s + g_pw[b];
        if (t > g_lim) break;
        gen(k - 1, b, t);
    }
}

static int cmp_u64(const void *x, const void *y) {
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static void build(uint64_t lim) {
    int b;
    size_t i, m;
    g_lim = lim;
    g_maxb = 0;
    for (b = 1; b < 80; b++) {
        uint64_t p = 1;
        int j;
        for (j = 0; j < 7; j++) p *= (uint64_t)b;
        if (p > lim) break;
        g_pw[b] = p;
        g_maxb = b;
    }
    g_buf = NULL;
    g_cnt = 0;
    g_cap = 0;
    gen(9, g_maxb, 0);
    qsort(g_buf, g_cnt, sizeof(uint64_t), cmp_u64);
    m = 0;
    for (i = 0; i < g_cnt; i++) {
        if (m == 0 || g_buf[m - 1] != g_buf[i]) g_buf[m++] = g_buf[i];
    }
    free(cache);
    cache = g_buf;
    cache_len = (idx_t)m;
}

seq_t a(idx_t n) {
    uint64_t lim = (uint64_t)1 << 16;
    if (cache && n <= cache_len) return (seq_t)cache[n - 1];
    for (;;) {
        build(lim);
        if (n <= cache_len) break;
        lim *= 4;
    }
    return (seq_t)cache[n - 1];
}
