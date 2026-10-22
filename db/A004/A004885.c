/*
 * A004885  Numbers that are the sum of at most 12 nonzero 8th powers.
 * https://oeis.org/A004885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- enumerate all sums of at most 12 nonzero 8th powers up
 *            to a bound, sort, dedupe; a(n) is the n-th smallest (a(1) = 0)
 * Range:     u8 n <= 13, u16 n <= 445, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *s_vals = NULL;
static size_t s_count = 0, s_cap = 0;
static uint64_t s_limit = 0;
static uint64_t s_pw[64];
static int s_npw = 0;

static void push_val(uint64_t v) {
    if (s_count == s_cap) {
        s_cap = s_cap ? s_cap * 2 : 1024;
        s_vals = (uint64_t *)realloc(s_vals, s_cap * sizeof(uint64_t));
    }
    s_vals[s_count++] = v;
}

static void dfs(int maxb, int left, uint64_t sum) {
    int b;
    push_val(sum);
    if (left == 0) return;
    for (b = maxb; b >= 1; b--) {
        uint64_t p = s_pw[b];
        if (p > s_limit - sum) continue;
        dfs(b, left - 1, sum + p);
    }
}

static int cmp_u64(const void *x, const void *y) {
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static void build(uint64_t limit) {
    size_t i, k;
    uint64_t b;
    s_limit = limit;
    s_count = 0;
    s_npw = 0;
    for (b = 1; b < 64; b++) {
        uint64_t p = b * b;
        p = p * p;
        p = p * p;
        if (p > limit) break;
        s_pw[b] = p;
        s_npw = (int)b;
    }
    dfs(s_npw, 12, 0);
    qsort(s_vals, s_count, sizeof(uint64_t), cmp_u64);
    k = 0;
    for (i = 0; i < s_count; i++) {
        if (k == 0 || s_vals[i] != s_vals[k - 1]) s_vals[k++] = s_vals[i];
    }
    s_count = k;
}

seq_t a(idx_t n) {
    if (s_limit == 0) build((uint64_t)1 << 20);
    while ((uint64_t)s_count < n) build(s_limit * 4);
    return (seq_t)s_vals[n - 1];
}
