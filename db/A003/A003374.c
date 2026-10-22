/*
 * A003374  Numbers that are the sum of 7 positive 7th powers.
 * https://oeis.org/A003374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search -- for a limit L, enumerate all sums of 7 positive 7th
 *            powers <= L (nondecreasing bases), sort and deduplicate; double
 *            L until at least n terms exist
 * Range:     u8 n <= 2, u16 n <= 100, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   A3374_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/A3374_cmp.h"

static uint64_t *A3374_vals = NULL;
static size_t A3374_cnt = 0, A3374_cap = 0;
static uint64_t A3374_p7[600];
static int A3374_nb = 0;

static void A3374_push(uint64_t v) {
    if (A3374_cnt == A3374_cap) {
        size_t nc = A3374_cap ? A3374_cap * 2 : 4096;
        uint64_t *t = (uint64_t *)realloc(A3374_vals, nc * sizeof(uint64_t));
        if (!t) return;
        A3374_vals = t;
        A3374_cap = nc;
    }
    A3374_vals[A3374_cnt++] = v;
}

static void A3374_rec(int k, int minb, uint64_t sum, uint64_t L) {
    if (k == 0) { A3374_push(sum); return; }
    for (int b = minb; b < A3374_nb; b++) {
        uint64_t p = A3374_p7[b];
        if (sum + (uint64_t)k * p > L) break;
        A3374_rec(k - 1, b, sum + p, L);
    }
}

static void A3374_build(uint64_t L) {
    A3374_nb = 1;
    A3374_p7[0] = 0;
    for (int b = 1; b < 600; b++) {
        uint64_t p = 1;
        int ok = 1;
        for (int i = 0; i < 7; i++) {
            if (p > L / (uint64_t)b) { ok = 0; break; }
            p *= (uint64_t)b;
        }
        if (!ok || p > L) break;
        A3374_p7[b] = p;
        A3374_nb = b + 1;
    }
    A3374_cnt = 0;
    A3374_rec(7, 1, 0, L);
    qsort(A3374_vals, A3374_cnt, sizeof(uint64_t), A3374_cmp);
    size_t u = 0;
    for (size_t i = 0; i < A3374_cnt; i++)
        if (u == 0 || A3374_vals[i] != A3374_vals[u - 1]) A3374_vals[u++] = A3374_vals[i];
    A3374_cnt = u;
}

seq_t a(idx_t n) {
    static uint64_t limit = 0;
    if (limit == 0 || A3374_cnt < n) {
        uint64_t L = limit ? limit : ((uint64_t)1 << 20);
        for (;;) {
            A3374_build(L);
            limit = L;
            if (A3374_cnt >= n) break;
            if (L > ((uint64_t)1 << 61)) break;
            L *= 2;
        }
    }
    if (n == 0 || A3374_cnt < n) return (seq_t)0;
    return (seq_t)A3374_vals[n - 1];
}
