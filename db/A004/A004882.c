/*
 * A004882  Numbers that are the sum of at most 9 nonzero 8th powers.
 * https://oeis.org/A004882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: enumerate all sums of at most 9 eighth powers b^8 (b >=
 *            1) up to a bound, sort and deduplicate; double the bound until n
 *            terms exist
 * Range:     u8 n <= 10, u16 n <= 220, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a4882_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/a4882_cmp.h"

static uint64_t *a4882_buf = NULL;
static size_t a4882_len = 0, a4882_cap = 0;
static uint64_t a4882_pw[64];
static int a4882_npw = 0;
static uint64_t a4882_lim = 0;

static void a4882_push(uint64_t v) {
    if (a4882_len == a4882_cap) {
        size_t nc = a4882_cap ? a4882_cap * 2 : 1024;
        a4882_buf = (uint64_t *)realloc(a4882_buf, nc * sizeof(uint64_t));
        a4882_cap = nc;
    }
    a4882_buf[a4882_len++] = v;
}

static void a4882_rec(int start, int remaining, uint64_t sum) {
    a4882_push(sum);
    if (remaining == 0) return;
    for (int i = start; i < a4882_npw; i++) {
        if (a4882_pw[i] > a4882_lim - sum) break;
        a4882_rec(i, remaining - 1, sum + a4882_pw[i]);
    }
}

seq_t a(idx_t n) {
    static uint64_t *terms = NULL;
    static size_t nterms = 0;
    static uint64_t lim = 512;

    while ((idx_t)nterms < n) {
        lim *= 2;
        a4882_lim = lim;
        a4882_npw = 0;
        for (uint64_t b = 1;; b++) {
            uint64_t p = b * b; p *= p; p *= p;
            if (p > lim) break;
            a4882_pw[a4882_npw++] = p;
        }
        a4882_len = 0;
        a4882_rec(0, 9, 0);
        qsort(a4882_buf, a4882_len, sizeof(uint64_t), a4882_cmp);
        size_t m = 0;
        for (size_t i = 0; i < a4882_len; i++) {
            if (m == 0 || a4882_buf[i] != a4882_buf[m - 1])
                a4882_buf[m++] = a4882_buf[i];
        }
        free(terms);
        terms = (uint64_t *)malloc((m ? m : 1) * sizeof(uint64_t));
        for (size_t i = 0; i < m; i++) terms[i] = a4882_buf[i];
        nterms = m;
    }
    uint64_t v = terms[n - 1];
    seq_t r = (seq_t)(v >> 32);
    r = r << 16;
    r = r << 16;
    r += (seq_t)(uint32_t)(v & 0xFFFFFFFFu);
    return r;
}
