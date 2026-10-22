/*
 * A004879  Numbers that are the sum of at most 6 nonzero 8th powers.
 * https://oeis.org/A004879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- enumerate all sums of at most 6 nonzero 8th powers up
 *            to a bound, sort and remove duplicates, and increase the bound
 *            until there are at least n distinct sums
 * Range:     u8 n <= 7, u16 n <= 84, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *g_buf = NULL;
static size_t g_cnt = 0, g_cap = 0;

static void push_val(uint64_t v) {
    if (g_cnt == g_cap) {
        size_t nc = g_cap ? g_cap * 2 : 4096;
        g_buf = (uint64_t *)realloc(g_buf, nc * sizeof(uint64_t));
        g_cap = nc;
    }
    g_buf[g_cnt++] = v;
}

static void gen(const uint64_t *p8, int nb, int k, int maxb, uint64_t cur, uint64_t lim) {
    int b;
    push_val(cur);
    if (k == 0) return;
    for (b = 1; b <= maxb && b < nb; b++) {
        uint64_t s = cur + p8[b];
        if (s > lim) break;
        gen(p8, nb, k - 1, b, s, lim);
    }
}

static int cmp_u64(const void *x, const void *y) {
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

seq_t a(idx_t n) {
    static uint64_t *cache = NULL;
    static size_t cache_len = 0;
    static uint64_t lim = (uint64_t)1 << 16;

    while (cache_len < n && lim < ((uint64_t)1 << 60)) {
        uint64_t p8[256];
        int nb = 0;
        size_t i, u;
        lim <<= 2;
        for (;;) {
            uint64_t v = 1, b = (uint64_t)nb;
            int e;
            for (e = 0; e < 8; e++) v *= b;
            if (v > lim) break;
            p8[nb++] = v;
            if (nb >= 256) break;
        }
        g_cnt = 0;
        gen(p8, nb, 6, nb - 1, 0, lim);
        qsort(g_buf, g_cnt, sizeof(uint64_t), cmp_u64);
        u = 0;
        for (i = 0; i < g_cnt; i++)
            if (u == 0 || g_buf[i] != g_buf[u - 1]) g_buf[u++] = g_buf[i];
        free(cache);
        cache = (uint64_t *)malloc((u ? u : 1) * sizeof(uint64_t));
        for (i = 0; i < u; i++) cache[i] = g_buf[i];
        cache_len = u;
    }
    if (n >= 1 && n <= cache_len) return (seq_t)cache[n - 1];
    return (seq_t)0;
}
