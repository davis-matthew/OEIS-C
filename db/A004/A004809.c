/*
 * A004809  Numbers that are the sum of 9 positive 10th powers.
 * https://oeis.org/A004809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search -- enumerate all multisets of 9 bases b1<=...<=b9 <= B,
 *            keep the sums below (B+1)^10 + 8 (so the list is complete up to
 *            there), sort and remove duplicates; increase B until there are
 *            at least n terms
 * Range:     u8 n <= 1, u16 n <= 17, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/u128_to_seq.h"

static uint64_t P10[64];
static uint64_t *buf = NULL;
static size_t buf_len = 0, buf_cap = 0;

static uint64_t *list = NULL;
static idx_t list_len = 0;
static int curB = 7;

static void push_val(uint64_t v) {
    if (buf_len == buf_cap) {
        buf_cap = buf_cap ? buf_cap * 2 : 1024;
        buf = (uint64_t *)realloc(buf, buf_cap * sizeof(uint64_t));
    }
    buf[buf_len++] = v;
}

static void rec(int k, int start, int B, uint64_t s, uint64_t lim) {
    if (k == 0) {
        push_val(s);
        return;
    }
    for (int b = start; b <= B; b++) {
        /* remaining k parts are all >= b */
        if (s + (uint64_t)k * P10[b] >= lim) break;
        rec(k - 1, b, B, s + P10[b], lim);
    }
}

static int cmp_u64(const void *x, const void *y) {
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static void build(int B) {
    for (int b = 0; b <= B + 1; b++) {
        uint64_t r = 1;
        for (int i = 0; i < 10; i++) r *= (uint64_t)b;
        P10[b] = r;
    }
    uint64_t lim = P10[B + 1] + 8; /* any sum using base > B is >= this */
    buf_len = 0;
    rec(9, 1, B, 0, lim);
    qsort(buf, buf_len, sizeof(uint64_t), cmp_u64);
    size_t u = 0;
    for (size_t i = 0; i < buf_len; i++)
        if (u == 0 || buf[i] != buf[u - 1]) buf[u++] = buf[i];
    free(list);
    list = (uint64_t *)malloc((u ? u : 1) * sizeof(uint64_t));
    for (size_t i = 0; i < u; i++) list[i] = buf[i];
    list_len = (idx_t)u;
}

seq_t a(idx_t n) {
    while (list_len < n && curB < 30) {
        curB++;
        build(curB);
    }
    if (n >= 1 && n <= list_len) return u128_to_seq((unsigned __int128)list[n - 1]);
    return 0;
}
