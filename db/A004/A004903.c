/*
 * A004903  Numbers that are the sum of at most 8 nonzero 10th powers.
 * https://oeis.org/A004903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve/enumeration - list every sum of at most 8 nonzero 10th
 *            powers up to a bound (doubling the bound as needed), sort,
 *            remove duplicates; a(n) is the n-th one
 * Range:     u8 n <= 9, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static uint64_t *s_list = NULL;
static idx_t s_count = 0;

static uint64_t *g_buf = NULL;
static idx_t g_len = 0;
static idx_t g_cap = 0;
static uint64_t g_limit = 0;
static uint64_t g_pw[64];
static int g_npw = 0;

static void push_sum(uint64_t v) {
    if (g_len == g_cap) {
        g_cap = g_cap ? g_cap * 2 : 4096;
        g_buf = (uint64_t *)realloc(g_buf, (size_t)g_cap * sizeof(uint64_t));
    }
    g_buf[g_len++] = v;
}

static void gen(uint64_t cur, int left, int minb) {
    int b;
    push_sum(cur);
    if (left == 0) return;
    for (b = minb; b < g_npw; b++) {
        if (g_pw[b] > g_limit - cur) break;
        gen(cur + g_pw[b], left - 1, b);
    }
}

static int cmp_u64(const void *x, const void *y) {
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static void build(uint64_t limit) {
    uint64_t b;
    idx_t i, k;
    g_limit = limit;
    g_npw = 0;
    for (b = 1; g_npw < 64; b++) {
        uint64_t p = 1;
        int e;
        bool over = false;
        for (e = 0; e < 10; e++) {
            if (p > limit / b) { over = true; break; }
            p *= b;
        }
        if (over || p > limit) break;
        g_pw[g_npw++] = p;
    }
    g_len = 0;
    gen(0, 8, 0);
    qsort(g_buf, (size_t)g_len, sizeof(uint64_t), cmp_u64);
    k = 0;
    for (i = 0; i < g_len; i++)
        if (k == 0 || g_buf[i] != g_buf[k - 1]) g_buf[k++] = g_buf[i];
    free(s_list);
    s_list = (uint64_t *)malloc((size_t)(k ? k : 1) * sizeof(uint64_t));
    for (i = 0; i < k; i++) s_list[i] = g_buf[i];
    s_count = k;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (s_count < n) {
        uint64_t limit = g_limit ? g_limit : ((uint64_t)1 << 16);
        while (s_count < n) {
            limit *= 2;
            build(limit);
        }
    }
    return (seq_t)s_list[n - 1];
}
