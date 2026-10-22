/*
 * A003386  Numbers that are the sum of 8 nonzero 8th powers.
 * https://oeis.org/A003386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search -- enumerate all sums of 8 nondecreasing positive 8th
 *            powers up to a bound X, sort, dedupe; double X until at least n
 *            terms
 * Range:     u8 n <= 1, u16 n <= 45, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   A3386_cmpu
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/A3386_cmpu.h"

static uint64_t *A3386_buf = NULL;
static size_t A3386_cnt = 0, A3386_cap = 0;
static uint64_t A3386_X = 0;
static uint64_t A3386_p8[64];
static int A3386_maxb = 0;

static void A3386_push(uint64_t v) {
    if (A3386_cnt == A3386_cap) {
        size_t nc = A3386_cap ? A3386_cap * 2 : 1024;
        A3386_buf = (uint64_t *)realloc(A3386_buf, nc * sizeof(uint64_t));
        A3386_cap = nc;
    }
    A3386_buf[A3386_cnt++] = v;
}

static void A3386_rec(int k, int minb, uint64_t s) {
    int b;
    if (k == 0) { A3386_push(s); return; }
    for (b = minb; b <= A3386_maxb; b++) {
        if (s + (uint64_t)k * A3386_p8[b] > A3386_X) break;
        A3386_rec(k - 1, b, s + A3386_p8[b]);
    }
}

static void A3386_build(uint64_t X) {
    int b;
    size_t i, u;
    A3386_X = X;
    A3386_cnt = 0;
    A3386_p8[0] = 0;
    A3386_maxb = 0;
    for (b = 1; b < 63; b++) {
        uint64_t v = 1;
        int j;
        for (j = 0; j < 8; j++) v *= (uint64_t)b;
        A3386_p8[b] = v;
        if (v > X) break;
        A3386_maxb = b;
    }
    A3386_rec(8, 1, 0);
    if (A3386_cnt == 0) return;
    qsort(A3386_buf, A3386_cnt, sizeof(uint64_t), A3386_cmpu);
    u = 1;
    for (i = 1; i < A3386_cnt; i++)
        if (A3386_buf[i] != A3386_buf[u - 1]) A3386_buf[u++] = A3386_buf[i];
    A3386_cnt = u;
}

seq_t a(idx_t n) {
    uint64_t X = A3386_X ? A3386_X : ((uint64_t)1 << 16);
    if (A3386_X == 0) A3386_build(X);
    while ((idx_t)A3386_cnt < n) {
        X *= 2;
        A3386_build(X);
    }
    return (seq_t)A3386_buf[n - 1];
}
