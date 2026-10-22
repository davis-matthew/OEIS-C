/*
 * A003376  Numbers that are the sum of 9 positive 7th powers.
 * https://oeis.org/A003376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search - list every sum of 9 positive 7th powers up to a bound L
 *            (nondecreasing bases, pruned by L), sort, remove duplicates,
 *            double L until there are at least n terms
 * Range:     u8 n <= 2, u16 n <= 164, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a3376_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a3376_cmp.h"

static uint64_t *a3376_vals = NULL;
static idx_t a3376_cnt = 0;
static uint64_t a3376_L = 0;

static uint64_t a3376_p[200];
static int a3376_np = 0;

static uint64_t *a3376_buf = NULL;
static idx_t a3376_blen = 0, a3376_bcap = 0;

static void a3376_push(uint64_t v) {
    if (a3376_blen >= a3376_bcap) {
        idx_t nc = a3376_bcap ? a3376_bcap * 2 : 4096;
        a3376_buf = (uint64_t *)realloc(a3376_buf, (size_t)nc * sizeof(uint64_t));
        a3376_bcap = nc;
    }
    a3376_buf[a3376_blen++] = v;
}

static void a3376_rec(int k, int minb, uint64_t sum, uint64_t L) {
    int b;
    if (k == 0) { a3376_push(sum); return; }
    for (b = minb; b < a3376_np; b++) {
        if (sum + (uint64_t)k * a3376_p[b] > L) break;
        a3376_rec(k - 1, b, sum + a3376_p[b], L);
    }
}

static void a3376_build(uint64_t L) {
    int b;
    idx_t i, w;
    a3376_np = 0;
    for (b = 1; b < 200; b++) {
        uint64_t q = (uint64_t)b, p = q * q * q;
        p = p * p * q;
        if (p > L) break;
        a3376_p[a3376_np++] = p;
    }
    a3376_blen = 0;
    a3376_rec(9, 0, 0, L);
    qsort(a3376_buf, (size_t)a3376_blen, sizeof(uint64_t), a3376_cmp);
    w = 0;
    for (i = 0; i < a3376_blen; i++)
        if (w == 0 || a3376_buf[i] != a3376_buf[w - 1])
            a3376_buf[w++] = a3376_buf[i];
    free(a3376_vals);
    a3376_vals = (uint64_t *)malloc((size_t)(w ? w : 1) * sizeof(uint64_t));
    for (i = 0; i < w; i++) a3376_vals[i] = a3376_buf[i];
    a3376_cnt = w;
    a3376_L = L;
}

seq_t a(idx_t n) {
    uint64_t L;
    if (n < 1) return (seq_t)0;
    if (a3376_cnt < n) {
        L = a3376_L ? a3376_L * 2 : ((uint64_t)1 << 20);
        for (;;) {
            a3376_build(L);
            if (a3376_cnt >= n) break;
            L *= 2;
        }
    }
    return (seq_t)a3376_vals[n - 1];
}
