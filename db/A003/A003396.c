/*
 * A003396  Numbers that are the sum of 7 positive 9th powers.
 * https://oeis.org/A003396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search -- enumerate all sums of 7 positive 9th powers with bases
 *            <= B, keep sums <= (B+1)^9 (all such sums are complete), sort
 *            and deduplicate; increase B as needed
 * Range:     u8 n <= 1, u16 n <= 26, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a3396_cmpu64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a3396_cmpu64.h"

static uint64_t *a3396_vals = NULL;
static idx_t a3396_len = 0;

static uint64_t *a3396_buf = NULL;
static size_t a3396_cnt = 0;
static size_t a3396_cap = 0;

static void a3396_push(uint64_t v) {
    if (a3396_cnt == a3396_cap) {
        size_t nc = a3396_cap ? a3396_cap * 2 : 1024;
        a3396_buf = (uint64_t *)realloc(a3396_buf, nc * sizeof(uint64_t));
        a3396_cap = nc;
    }
    a3396_buf[a3396_cnt++] = v;
}

static void a3396_rec(const uint64_t *p9, int B, int k, int minb, uint64_t sum, uint64_t limit) {
    int b;
    if (k == 0) {
        a3396_push(sum);
        return;
    }
    for (b = minb; b <= B; b++) {
        /* remaining k terms are each >= p9[b] */
        if (sum + (uint64_t)k * p9[b] > limit) break;
        a3396_rec(p9, B, k - 1, b, sum + p9[b], limit);
    }
}

static void a3396_build(int B) {
    uint64_t p9[128];
    uint64_t limit;
    int i, j;
    size_t u, w;
    for (i = 0; i <= B + 1; i++) {
        uint64_t v = 1;
        for (j = 0; j < 9; j++) v *= (uint64_t)i;
        p9[i] = v;
    }
    limit = p9[B + 1];
    a3396_cnt = 0;
    a3396_rec(p9, B, 7, 1, 0, limit);
    qsort(a3396_buf, a3396_cnt, sizeof(uint64_t), a3396_cmpu64);
    w = 0;
    for (u = 0; u < a3396_cnt; u++) {
        if (w == 0 || a3396_buf[u] != a3396_buf[w - 1]) a3396_buf[w++] = a3396_buf[u];
    }
    free(a3396_vals);
    a3396_vals = (uint64_t *)malloc((w ? w : 1) * sizeof(uint64_t));
    for (u = 0; u < w; u++) a3396_vals[u] = a3396_buf[u];
    a3396_len = (idx_t)w;
}

seq_t a(idx_t n) {
    static int B = 0;
    if (n < 1) n = 1;
    if (a3396_vals == NULL) {
        B = 16;
        a3396_build(B);
    }
    while (a3396_len < n && B < 100) {
        B += 8;
        a3396_build(B);
    }
    if (n > a3396_len) return (seq_t)0;
    return (seq_t)a3396_vals[n - 1];
}
