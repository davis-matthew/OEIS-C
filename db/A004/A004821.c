/*
 * A004821  Numbers that are the sum of 10 positive 11th powers.
 * https://oeis.org/A004821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search -- enumerate all sums of 10 positive 11th powers up to a
 *            bound (bases nondecreasing), sort and deduplicate; double the
 *            bound until at least n terms are found; a(n) is the n-th
 *            smallest
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 5473, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/u128_to_seq.h"

#define NB 50

static uint64_t p11[NB + 1];
static bool p_ready = false;

static uint64_t *list = NULL;
static idx_t list_len = 0;
static idx_t list_cap = 0;
static uint64_t cur_limit = 0;

static void init_pows(void) {
    int b, k;
    if (p_ready) return;
    p11[0] = 0;
    for (b = 1; b <= NB; b++) {
        uint64_t v = 1;
        for (k = 0; k < 11; k++) v *= (uint64_t)b;
        p11[b] = v;
    }
    p_ready = true;
}

static void push(uint64_t v) {
    if (list_len == list_cap) {
        idx_t nc = list_cap ? list_cap * 2 : 1024;
        list = (uint64_t *)realloc(list, nc * sizeof(uint64_t));
        list_cap = nc;
    }
    list[list_len++] = v;
}

static void dfs(int r, int minb, uint64_t s, uint64_t L) {
    int b;
    if (r == 0) {
        push(s);
        return;
    }
    for (b = minb; b <= NB; b++) {
        if (p11[b] > (L - s) / (uint64_t)r) break;
        dfs(r - 1, b, s + p11[b], L);
    }
}

static int cmp_u64(const void *x, const void *y) {
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    if (u < v) return -1;
    if (u > v) return 1;
    return 0;
}

static void build(uint64_t L) {
    idx_t i, m;
    list_len = 0;
    dfs(10, 1, 0, L);
    qsort(list, list_len, sizeof(uint64_t), cmp_u64);
    m = 0;
    for (i = 0; i < list_len; i++) {
        if (m == 0 || list[i] != list[m - 1]) list[m++] = list[i];
    }
    list_len = m;
    cur_limit = L;
}

seq_t a(idx_t n) {
    init_pows();
    if (n == 0) return 0;
    if (cur_limit == 0) build((uint64_t)1 << 20);
    while (list_len < n && cur_limit < ((uint64_t)1 << 62)) {
        build(cur_limit * 2);
    }
    if (n > list_len) return 0;
    return u128_to_seq((unsigned __int128)list[n - 1]);
}
