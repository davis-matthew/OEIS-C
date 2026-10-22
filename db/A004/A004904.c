/*
 * A004904  Numbers that are the sum of at most 9 nonzero 10th powers.
 * https://oeis.org/A004904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: build every sum of at most 9 tenth powers k^10 with 1 <=
 *            k <= B (0 counts as the empty sum), keep the distinct values
 *            below (B+1)^10, sort them, and make B larger until there are at
 *            least n terms
 * Range:     u8 n <= 10, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   pow10u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow10u.h"

static uint64_t *g_buf = NULL;
static size_t g_len = 0, g_cap = 0;

static void push_val(uint64_t v) {
    if (g_len == g_cap) {
        g_cap = g_cap ? g_cap * 2 : 1024;
        g_buf = (uint64_t *)realloc(g_buf, g_cap * sizeof(uint64_t));
    }
    g_buf[g_len++] = v;
}

static void gen_sums(uint64_t cur, int remaining, int base, int maxb,
                     const uint64_t *pw, uint64_t limit) {
    if (cur >= limit) return;
    if (base > maxb || remaining == 0) {
        push_val(cur);
        return;
    }
    for (int c = 0; c <= remaining; c++) {
        uint64_t v = cur + (uint64_t)c * pw[base];
        if (v >= limit) break;
        gen_sums(v, remaining - c, base + 1, maxb, pw, limit);
    }
}

static int cmp_u64(const void *x, const void *y) {
    uint64_t p = *(const uint64_t *)x, q = *(const uint64_t *)y;
    return (p > q) - (p < q);
}

seq_t a(idx_t n) {
    static uint64_t *terms = NULL;
    static size_t nterms = 0;
    static int curB = 1;

    while ((idx_t)nterms < n) {
        int B = curB + 1;
        uint64_t pw[64];
        for (int k = 1; k <= B; k++) pw[k] = pow10u((uint64_t)k);
        uint64_t limit = pow10u((uint64_t)(B + 1));

        g_len = 0;
        gen_sums(0, 9, 1, B, pw, limit);
        qsort(g_buf, g_len, sizeof(uint64_t), cmp_u64);

        size_t u = 0;
        for (size_t i = 0; i < g_len; i++)
            if (u == 0 || g_buf[i] != g_buf[u - 1]) g_buf[u++] = g_buf[i];

        free(terms);
        terms = (uint64_t *)malloc((u ? u : 1) * sizeof(uint64_t));
        for (size_t i = 0; i < u; i++) terms[i] = g_buf[i];
        nterms = u;
        curB = B;
    }

    return (seq_t)terms[n - 1];
}
