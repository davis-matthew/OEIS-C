/*
 * A004857  Numbers that are the sum of at most 6 nonzero 6th powers.
 * https://oeis.org/A004857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- enumerate all sums of at most 6 nonzero 6th powers up
 *            to a bound X (nondecreasing bases), sort, deduplicate; double X
 *            until n terms exist
 * Range:     u8 n <= 22, u16 n <= 611, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a4857_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/a4857_cmp.h"

static uint64_t *a4857_buf = NULL;
static size_t a4857_cnt = 0, a4857_cap = 0;

static uint64_t *a4857_list = NULL;
static size_t a4857_len = 0;
static uint64_t a4857_bound = 0;

static void a4857_push(uint64_t v) {
    if (a4857_cnt == a4857_cap) {
        a4857_cap = a4857_cap ? a4857_cap * 2 : 1024;
        a4857_buf = (uint64_t *)realloc(a4857_buf, a4857_cap * sizeof(uint64_t));
    }
    a4857_buf[a4857_cnt++] = v;
}

static void a4857_gen(uint64_t s, size_t start, int left,
                      const uint64_t *pw, size_t np, uint64_t X) {
    size_t i;
    a4857_push(s);
    if (left == 0) return;
    for (i = start; i < np; i++) {
        if (pw[i] > X - s) break;
        a4857_gen(s + pw[i], i, left - 1, pw, np, X);
    }
}

static void a4857_build(uint64_t X) {
    uint64_t pw[1700];
    size_t np = 0, i, u;
    uint64_t b;
    for (b = 1; b < 1600; b++) {
        uint64_t p = b * b * b;
        p = p * p;
        if (p > X) break;
        pw[np++] = p;
    }
    a4857_cnt = 0;
    a4857_gen(0, 0, 6, pw, np, X);
    qsort(a4857_buf, a4857_cnt, sizeof(uint64_t), a4857_cmp);
    u = 0;
    for (i = 0; i < a4857_cnt; i++) {
        if (u == 0 || a4857_buf[i] != a4857_buf[u - 1]) a4857_buf[u++] = a4857_buf[i];
    }
    free(a4857_list);
    a4857_list = (uint64_t *)malloc((u ? u : 1) * sizeof(uint64_t));
    for (i = 0; i < u; i++) a4857_list[i] = a4857_buf[i];
    a4857_len = u;
    a4857_bound = X;
}

seq_t a(idx_t n) {
    uint64_t X;
    if (n < 1) return (seq_t)0;
    X = a4857_bound ? a4857_bound : 4096;
    if (a4857_bound == 0) a4857_build(X);
    while ((uint64_t)a4857_len < (uint64_t)n) {
        X *= 2;
        a4857_build(X);
    }
    return (seq_t)a4857_list[n - 1];
}
