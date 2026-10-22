/*
 * A004880  Numbers that are the sum of at most 7 nonzero 8th powers.
 * https://oeis.org/A004880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- enumerate all sums of at most 7 nonzero 8th powers up
 *            to a bound B (doubling B as needed), sort, deduplicate; a(n) is
 *            the n-th smallest (a(1) = 0, the empty sum)
 * Range:     u8 n <= 8, u16 n <= 120, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *cache = NULL;
static idx_t cache_len = 0;

static uint64_t *g_buf = NULL;
static size_t g_cnt = 0, g_cap = 0;
static uint64_t g_pw[256];
static int g_np = 0;
static uint64_t g_bound = 0;

static int cmp_u64(const void *x, const void *y) {
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static void push(uint64_t s) {
    if (g_cnt == g_cap) {
        g_cap = g_cap ? g_cap * 2 : 1024;
        g_buf = (uint64_t *)realloc(g_buf, g_cap * sizeof(uint64_t));
    }
    g_buf[g_cnt++] = s;
}

static void rec(int start, int remaining, uint64_t sum) {
    push(sum);
    if (remaining == 0) return;
    for (int i = start; i < g_np; i++) {
        uint64_t p = g_pw[i];
        if (p > g_bound - sum) break;
        rec(i, remaining - 1, sum + p);
    }
}

static void build(uint64_t bound) {
    g_bound = bound;
    g_np = 0;
    for (uint64_t b = 1; b < 256; b++) {
        uint64_t p = b * b; p *= p; p *= p;   /* b^8, b < 256 fits in 64 bits */
        if (p > bound) break;
        g_pw[g_np++] = p;
    }
    g_cnt = 0;
    rec(0, 7, 0);
    qsort(g_buf, g_cnt, sizeof(uint64_t), cmp_u64);
    size_t u = 0;
    for (size_t i = 0; i < g_cnt; i++)
        if (u == 0 || g_buf[i] != g_buf[u - 1]) g_buf[u++] = g_buf[i];
    free(cache);
    cache = (uint64_t *)malloc((u ? u : 1) * sizeof(uint64_t));
    for (size_t i = 0; i < u; i++) cache[i] = g_buf[i];
    cache_len = (idx_t)u;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    uint64_t bound = g_bound ? g_bound : ((uint64_t)1 << 16);
    if (cache == NULL) build(bound);
    while (cache_len < n && bound < ((uint64_t)1 << 61)) {
        bound *= 2;
        build(bound);
    }
    if (n > cache_len) return (seq_t)0;
    return (seq_t)cache[n - 1];
}
