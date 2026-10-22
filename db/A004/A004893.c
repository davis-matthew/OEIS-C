/*
 * A004893  Numbers that are the sum of at most 9 positive 9th powers.
 * https://oeis.org/A004893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search - list every sum of at most 9 ninth powers k^9 with 1 <=
 *            k <= B that is below (B+1)^9 (that part of the list is
 *            complete), sort and dedup, and raise B until there are at least
 *            n values; a(n) is the n-th smallest
 * Range:     u8 n <= 10, u16 n <= 164, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/u128_to_seq.h"

static uint64_t *vals = NULL;
static idx_t vals_len = 0;
static idx_t vals_cap = 0;
static uint64_t cur_B = 0;
static uint64_t pw[256];

static int cmp_u64(const void *x, const void *y) {
    uint64_t p = *(const uint64_t *)x, q = *(const uint64_t *)y;
    return (p > q) - (p < q);
}

static void push(uint64_t v) {
    if (vals_len == vals_cap) {
        idx_t nc = vals_cap ? vals_cap * 2 : 1024;
        vals = (uint64_t *)realloc(vals, nc * sizeof(uint64_t));
        vals_cap = nc;
    }
    vals[vals_len++] = v;
}

/* each multiset (nonincreasing bases) visited once */
static void dfs(uint64_t maxb, int remain, uint64_t cur, uint64_t limit) {
    push(cur);
    if (remain == 0) return;
    for (uint64_t b = maxb; b >= 1; b--) {
        if (pw[b] >= limit - cur) continue; /* cur + pw[b] >= limit */
        dfs(b, remain - 1, cur + pw[b], limit);
    }
}

static uint64_t pow9(uint64_t k) {
    uint64_t p = 1;
    for (int i = 0; i < 9; i++) p *= k;
    return p;
}

static void build(uint64_t B) {
    for (uint64_t k = 1; k <= B + 1; k++) pw[k] = pow9(k);
    uint64_t limit = pw[B + 1];
    vals_len = 0;
    dfs(B, 9, 0, limit);
    qsort(vals, vals_len, sizeof(uint64_t), cmp_u64);
    idx_t j = 0;
    for (idx_t i = 0; i < vals_len; i++)
        if (j == 0 || vals[j - 1] != vals[i]) vals[j++] = vals[i];
    vals_len = j;
    cur_B = B;
}

seq_t a(idx_t n) {
    if (cur_B == 0) build(1);
    while (vals_len < n && cur_B < 120) build(cur_B + 1);
    if (n < 1 || n > vals_len) return 0;
    return u128_to_seq((unsigned __int128)vals[n - 1]);
}
