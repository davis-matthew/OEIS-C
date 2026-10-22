/*
 * A004907  Numbers that are the sum of at most 12 nonzero 10th powers.
 * https://oeis.org/A004907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- enumerate all multisets of at most 12 nonzero 10th
 *            powers with sum <= L (L grown as needed), sort, deduplicate;
 *            a(n) = n-th smallest
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/u128_to_seq.h"

static uint64_t *cache = NULL;
static idx_t cache_len = 0;

static uint64_t pw[128];
static int npw;
static uint64_t *buf;
static size_t buf_len, buf_cap;
static uint64_t bound;

static void push(uint64_t v) {
    if (buf_len == buf_cap) {
        buf_cap = buf_cap ? buf_cap * 2 : 1024;
        buf = (uint64_t *)realloc(buf, buf_cap * sizeof(uint64_t));
    }
    buf[buf_len++] = v;
}

static void gen(int minb, uint64_t sum, int remaining) {
    push(sum);
    if (remaining == 0) return;
    for (int b = minb; b < npw; b++) {
        if (pw[b] > bound - sum) break;
        gen(b, sum + pw[b], remaining - 1);
    }
}

static int cmp_u64(const void *x, const void *y) {
    uint64_t p = *(const uint64_t *)x, q = *(const uint64_t *)y;
    return (p > q) - (p < q);
}

static void build(uint64_t L) {
    bound = L;
    npw = 0;
    for (uint64_t b = 1; npw < 128; b++) {
        unsigned __int128 p = 1;
        for (int j = 0; j < 10; j++) p *= b;
        if (p > (unsigned __int128)L) break;
        pw[npw++] = (uint64_t)p;
    }
    buf = NULL;
    buf_len = buf_cap = 0;
    gen(0, 0, 12);
    qsort(buf, buf_len, sizeof(uint64_t), cmp_u64);
    size_t m = 0;
    for (size_t i = 0; i < buf_len; i++)
        if (m == 0 || buf[i] != buf[m - 1]) buf[m++] = buf[i];
    free(cache);
    cache = buf;
    cache_len = (idx_t)m;
}

seq_t a(idx_t n) {
    static uint64_t L = 0;
    if (L == 0) {
        L = 1 << 16;
        build(L);
    }
    while (cache_len < n) {
        L *= 4;
        build(L);
    }
    return u128_to_seq((unsigned __int128)cache[n - 1]);
}
