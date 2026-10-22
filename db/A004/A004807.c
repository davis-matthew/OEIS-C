/*
 * A004807  Numbers that are the sum of 7 positive 10th powers.
 * https://oeis.org/A004807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search - enumerate all sums of 7 positive 10th powers with bases
 *            1..m up to (m+1)^10 + 5 (that range is then complete), sort,
 *            dedupe; m grows until n terms exist
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/u128_to_seq.h"

typedef unsigned __int128 u128_t;

static u128_t *g_buf = NULL;
static size_t g_cnt = 0, g_cap = 0;

static void push_val(u128_t v) {
    if (g_cnt >= g_cap) {
        g_cap = g_cap ? g_cap * 2 : 4096;
        g_buf = (u128_t *)realloc(g_buf, g_cap * sizeof(u128_t));
    }
    g_buf[g_cnt++] = v;
}

static void gen(int left, idx_t start, idx_t m, const u128_t *pw, u128_t sum, u128_t lim) {
    if (left == 0) {
        push_val(sum);
        return;
    }
    for (idx_t b = start; b <= m; b++) {
        u128_t p = pw[b];
        if (sum + (u128_t)left * p > lim) break;
        gen(left - 1, b, m, pw, sum + p, lim);
    }
}

static int cmp_u128(const void *x, const void *y) {
    u128_t u = *(const u128_t *)x, v = *(const u128_t *)y;
    return (u < v) ? -1 : (u > v) ? 1 : 0;
}

seq_t a(idx_t n) {
    static u128_t *vals = NULL;
    static size_t nv = 0;
    static idx_t m = 8;

    if (n >= 1 && n <= nv) return u128_to_seq(vals[n - 1]);

    for (;;) {
        u128_t *pw = (u128_t *)malloc((size_t)(m + 2) * sizeof(u128_t));
        for (idx_t b = 0; b <= m + 1; b++) {
            u128_t p = 1;
            for (int k = 0; k < 10; k++) p *= (u128_t)b;
            pw[b] = p;
        }
        u128_t lim = pw[m + 1] + 5;

        g_buf = NULL;
        g_cnt = 0;
        g_cap = 0;
        gen(7, 1, m, pw, 0, lim);
        free(pw);

        if (g_cnt > 0) qsort(g_buf, g_cnt, sizeof(u128_t), cmp_u128);
        size_t u = 0;
        for (size_t i = 0; i < g_cnt; i++) {
            if (u == 0 || g_buf[i] != g_buf[u - 1]) g_buf[u++] = g_buf[i];
        }

        free(vals);
        vals = g_buf;
        nv = u;
        g_buf = NULL;
        g_cnt = 0;
        g_cap = 0;

        if (n <= nv) return u128_to_seq(vals[n - 1]);
        m += m / 2 + 1;
    }
}
