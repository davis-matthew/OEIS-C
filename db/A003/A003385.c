/*
 * A003385  Numbers that are the sum of 7 nonzero 8th powers.
 * https://oeis.org/A003385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search -- enumerate all sums of 7 nonzero 8th powers up to a
 *            bound (doubling the bound until at least n distinct sums are
 *            found), sort, dedupe, a(n) = n-th smallest distinct sum
 * Range:     u8 n <= 1, u16 n <= 36, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a3385_cmpu
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a3385_cmpu.h"

static uint64_t a3385_pw[201];
static uint64_t *a3385_buf = NULL;
static size_t a3385_len = 0, a3385_cap = 0;
static uint64_t a3385_lim = 0;

static uint64_t *a3385_vals = NULL;
static size_t a3385_nvals = 0;

static void a3385_push(uint64_t v) {
    if (a3385_len == a3385_cap) {
        size_t nc = a3385_cap ? a3385_cap * 2 : 1024;
        a3385_buf = (uint64_t *)realloc(a3385_buf, nc * sizeof(uint64_t));
        a3385_cap = nc;
    }
    a3385_buf[a3385_len++] = v;
}

static void a3385_walk(int k, int minb, uint64_t sum) {
    if (k == 0) { a3385_push(sum); return; }
    for (int b = minb; b <= 200; b++) {
        uint64_t p = a3385_pw[b];
        if (p > a3385_lim) break;
        if (sum + (uint64_t)k * p > a3385_lim) break;
        a3385_walk(k - 1, b, sum + p);
    }
}

static void a3385_build(uint64_t lim) {
    a3385_lim = lim;
    a3385_len = 0;
    a3385_walk(7, 1, 0);
    qsort(a3385_buf, a3385_len, sizeof(uint64_t), a3385_cmpu);
    size_t w = 0;
    for (size_t i = 0; i < a3385_len; i++)
        if (w == 0 || a3385_buf[i] != a3385_buf[w - 1]) a3385_buf[w++] = a3385_buf[i];
    a3385_vals = a3385_buf;
    a3385_nvals = w;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (a3385_pw[1] == 0) {
        for (int b = 0; b <= 200; b++) {
            uint64_t x = (uint64_t)b * (uint64_t)b;
            x = x * x;
            a3385_pw[b] = x * x;
        }
    }
    if (a3385_vals == NULL || (uint64_t)a3385_nvals < n) {
        uint64_t lim = a3385_lim ? a3385_lim : ((uint64_t)1 << 24);
        if (a3385_vals != NULL) lim *= 2;
        for (;;) {
            a3385_build(lim);
            if ((uint64_t)a3385_nvals >= n || lim >= ((uint64_t)1 << 60)) break;
            lim *= 2;
        }
    }
    if ((uint64_t)a3385_nvals < n) return 0;
    return (seq_t)a3385_vals[n - 1];
}
