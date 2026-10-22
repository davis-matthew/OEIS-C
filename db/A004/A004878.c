/*
 * A004878  Numbers that are the sum of at most 5 nonzero 8th powers.
 * https://oeis.org/A004878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: enumerate all sums of at most 5 nonzero 8th powers below
 *            (B+1)^8, sort, dedupe; a(n) = n-th smallest (a(1) = 0)
 * Range:     u8 n <= 6, u16 n <= 56, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   A004878_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/A004878_cmp.h"

static uint64_t *A004878_terms = NULL;
static size_t A004878_count = 0;

static uint64_t *A004878_buf = NULL;
static size_t A004878_len = 0;
static size_t A004878_cap = 0;
static uint64_t A004878_p8[256];

static void A004878_push(uint64_t v) {
    if (A004878_len == A004878_cap) {
        size_t nc = A004878_cap ? A004878_cap * 2 : 1024;
        A004878_buf = (uint64_t *)realloc(A004878_buf, nc * sizeof(uint64_t));
        A004878_cap = nc;
    }
    A004878_buf[A004878_len++] = v;
}

static void A004878_gen(int k, uint64_t minb, uint64_t B, uint64_t sum, uint64_t lim) {
    uint64_t b;
    A004878_push(sum);
    if (k == 5) return;
    for (b = minb; b <= B; b++) {
        uint64_t s = sum + A004878_p8[b];
        if (s >= lim) break;
        A004878_gen(k + 1, b, B, s, lim);
    }
}

static void A004878_build(uint64_t B) {
    uint64_t b, lim;
    size_t i, m;
    int j;
    for (b = 0; b <= B + 1; b++) {
        uint64_t p = 1;
        for (j = 0; j < 8; j++) p *= b;
        A004878_p8[b] = p;
    }
    lim = A004878_p8[B + 1];
    A004878_len = 0;
    A004878_gen(0, 1, B, 0, lim);
    qsort(A004878_buf, A004878_len, sizeof(uint64_t), A004878_cmp);
    m = 0;
    for (i = 0; i < A004878_len; i++)
        if (m == 0 || A004878_buf[i] != A004878_buf[m - 1])
            A004878_buf[m++] = A004878_buf[i];
    free(A004878_terms);
    A004878_terms = (uint64_t *)malloc((m ? m : 1) * sizeof(uint64_t));
    memcpy(A004878_terms, A004878_buf, m * sizeof(uint64_t));
    A004878_count = m;
}

seq_t a(idx_t n) {
    static uint64_t B = 0;
    if (B == 0) {
        B = 8;
        A004878_build(B);
    }
    while (A004878_count < n && B < 200) {
        B += 4;
        A004878_build(B);
    }
    return (seq_t)A004878_terms[n - 1];
}
