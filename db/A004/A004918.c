/*
 * A004918  Numbers that are the sum of at most 12 positive 11th powers.
 * https://oeis.org/A004918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- enumerate all sums of at most 12 positive 11th powers
 *            below m^11 (bases < m), sort, dedupe, growing m until at least n
 *            terms are known
 * Range:     u8 n <= 13, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/u128_to_seq.h"

typedef unsigned __int128 u128_t;

static u128_t *g_buf = NULL;
static size_t g_len = 0, g_cap = 0;

static void push_val(u128_t v) {
    if (g_len == g_cap) {
        size_t nc = g_cap ? g_cap * 2 : 1024;
        g_buf = (u128_t *)realloc(g_buf, nc * sizeof(u128_t));
        g_cap = nc;
    }
    g_buf[g_len++] = v;
}

static void rec(const u128_t *pw, int maxb, int left, u128_t sum, u128_t lim) {
    push_val(sum);
    if (left == 0) return;
    for (int b = 1; b <= maxb; b++) {
        u128_t s = sum + pw[b];
        if (s >= lim) break;
        rec(pw, b, left - 1, s, lim);
    }
}

static int cmp_u128(const void *x, const void *y) {
    u128_t a1 = *(const u128_t *)x, b1 = *(const u128_t *)y;
    return (a1 < b1) ? -1 : (a1 > b1) ? 1 : 0;
}

static u128_t *terms = NULL;
static size_t nterms = 0;
static int cur_m = 1;

seq_t a(idx_t n) {
    while ((idx_t)nterms < n) {
        int m = cur_m + 1;
        u128_t *pw = (u128_t *)malloc((size_t)(m + 1) * sizeof(u128_t));
        for (int b = 0; b <= m; b++) {
            u128_t p = 1;
            for (int i = 0; i < 11; i++) p *= (u128_t)b;
            pw[b] = p;
        }
        u128_t lim = pw[m];
        g_len = 0;
        rec(pw, m - 1, 12, 0, lim);
        free(pw);
        qsort(g_buf, g_len, sizeof(u128_t), cmp_u128);
        size_t w = 0;
        for (size_t i = 0; i < g_len; i++)
            if (w == 0 || g_buf[i] != g_buf[w - 1]) g_buf[w++] = g_buf[i];
        free(terms);
        terms = (u128_t *)malloc((w ? w : 1) * sizeof(u128_t));
        memcpy(terms, g_buf, w * sizeof(u128_t));
        nterms = w;
        cur_m = m;
    }
    return u128_to_seq(terms[n - 1]);
}
