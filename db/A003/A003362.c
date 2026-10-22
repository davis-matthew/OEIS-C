/*
 * A003362  Numbers that are the sum of 6 positive 6th powers.
 * https://oeis.org/A003362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search: list all sums of six 6th powers of nondecreasing bases
 *            up to a doubling bound, then sort and deduplicate
 * Range:     u8 n <= 4, u16 n <= 271, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a3362_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a3362_cmp.h"

static uint64_t *a3362_vals = NULL;
static size_t a3362_len = 0;
static uint64_t a3362_lim = 0;

static uint64_t *a3362_buf = NULL;
static size_t a3362_cnt = 0;
static size_t a3362_cap = 0;

static void a3362_push(uint64_t v) {
    if (a3362_cnt == a3362_cap) {
        a3362_cap = a3362_cap ? 2 * a3362_cap : 4096;
        a3362_buf = (uint64_t *)realloc(a3362_buf, a3362_cap * sizeof(uint64_t));
    }
    a3362_buf[a3362_cnt++] = v;
}

/* choose 'left' more bases, each >= b, keeping the sum <= lim */
static void a3362_rec(int left, uint64_t b, uint64_t sum, uint64_t lim) {
    if (left == 0) {
        a3362_push(sum);
        return;
    }
    for (;; b++) {
        uint64_t p = b * b * b;
        p = p * p;
        if (sum + (uint64_t)left * p > lim) break;
        a3362_rec(left - 1, b, sum + p, lim);
    }
}

static void a3362_build(uint64_t lim) {
    size_t i, u = 0;
    a3362_cnt = 0;
    a3362_rec(6, 1, 0, lim);
    qsort(a3362_buf, a3362_cnt, sizeof(uint64_t), a3362_cmp);
    for (i = 0; i < a3362_cnt; i++)
        if (u == 0 || a3362_buf[i] != a3362_buf[u - 1])
            a3362_buf[u++] = a3362_buf[i];
    free(a3362_vals);
    a3362_vals = (uint64_t *)malloc((u ? u : 1) * sizeof(uint64_t));
    for (i = 0; i < u; i++) a3362_vals[i] = a3362_buf[i];
    a3362_len = u;
    a3362_lim = lim;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    while ((idx_t)a3362_len < n) {
        uint64_t lim = a3362_lim ? a3362_lim * 2 : (uint64_t)1 << 16;
        a3362_build(lim);
    }
    return (seq_t)a3362_vals[n - 1];
}
