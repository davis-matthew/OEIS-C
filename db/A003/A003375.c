/*
 * A003375  Numbers that are the sum of 8 positive 7th powers.
 * https://oeis.org/A003375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search -- enumerate all nondecreasing 8-tuples of positive bases
 *            with sum of 7th powers <= X, sort and deduplicate; double X
 *            until n terms exist
 * Range:     u8 n <= 2, u16 n <= 130, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   A3375_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/A3375_cmp.h"

static uint64_t *A3375_vals = NULL;
static idx_t A3375_cnt = 0;
static uint64_t *A3375_buf = NULL;
static idx_t A3375_len = 0, A3375_cap = 0;
static uint64_t A3375_p7[400];

static void A3375_push(uint64_t v) {
    if (A3375_len == A3375_cap) {
        idx_t nc = A3375_cap ? A3375_cap * 2 : 4096;
        A3375_buf = (uint64_t *)realloc(A3375_buf, (size_t)nc * sizeof(uint64_t));
        A3375_cap = nc;
    }
    A3375_buf[A3375_len++] = v;
}

static void A3375_rec(int k, int minb, uint64_t sum, uint64_t X) {
    int b;
    if (k == 0) { A3375_push(sum); return; }
    for (b = minb; b < 400; b++) {
        uint64_t p = A3375_p7[b];
        if (sum + (uint64_t)k * p > X) break;
        A3375_rec(k - 1, b, sum + p, X);
    }
}

static void A3375_build(uint64_t X) {
    idx_t i, u = 0;
    A3375_len = 0;
    A3375_rec(8, 1, 0, X);
    qsort(A3375_buf, (size_t)A3375_len, sizeof(uint64_t), A3375_cmp);
    for (i = 0; i < A3375_len; i++)
        if (u == 0 || A3375_buf[i] != A3375_buf[u - 1]) A3375_buf[u++] = A3375_buf[i];
    free(A3375_vals);
    A3375_vals = (uint64_t *)malloc((size_t)(u ? u : 1) * sizeof(uint64_t));
    for (i = 0; i < u; i++) A3375_vals[i] = A3375_buf[i];
    A3375_cnt = u;
}

seq_t a(idx_t n) {
    static uint64_t X = 0;
    if (n < 1) return 0;
    if (X == 0) {
        int b, j;
        for (b = 0; b < 400; b++) {
            uint64_t p = 1;
            for (j = 0; j < 7; j++) p *= (uint64_t)b;
            A3375_p7[b] = p;
        }
        X = (uint64_t)1 << 16;
        A3375_build(X);
    }
    while (A3375_cnt < n) {
        X *= 2;
        A3375_build(X);
    }
    return (seq_t)A3375_vals[n - 1];
}
