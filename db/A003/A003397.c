/*
 * A003397  Numbers that are the sum of 8 positive 9th powers.
 * https://oeis.org/A003397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search -- enumerate all sums of 8 positive 9th powers up to a
 *            bound (doubling the bound as needed), sort, remove duplicates,
 *            take the n-th
 * Range:     u8 n <= 1, u16 n <= 30, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a3397_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a3397_cmp.h"

static uint64_t a3397_p9[200];
static int a3397_np = 0;
static uint64_t *a3397_buf = NULL;
static size_t a3397_len = 0, a3397_cap = 0;
static uint64_t a3397_L = 0;

static uint64_t *a3397_terms = NULL;
static size_t a3397_cnt = 0;

static void a3397_push(uint64_t v) {
    if (a3397_len == a3397_cap) {
        size_t nc = a3397_cap ? a3397_cap * 2 : 1024;
        a3397_buf = (uint64_t *)realloc(a3397_buf, nc * sizeof(uint64_t));
        a3397_cap = nc;
    }
    a3397_buf[a3397_len++] = v;
}

static void a3397_rec(int rem, int minb, uint64_t sum) {
    if (rem == 0) {
        a3397_push(sum);
        return;
    }
    uint64_t room = a3397_L - sum;
    for (int b = minb; b < a3397_np; b++) {
        if (a3397_p9[b] > room / (uint64_t)rem) break;
        a3397_rec(rem - 1, b, sum + a3397_p9[b]);
    }
}

static void a3397_build(uint64_t L) {
    a3397_L = L;
    a3397_np = 0;
    for (int b = 0; b < 200; b++) {
        uint64_t v = 1;
        int ok = 1;
        for (int i = 0; i < 9; i++) {
            if (b != 0 && v > L / (uint64_t)b) { ok = 0; break; }
            v *= (uint64_t)b;
        }
        if (!ok || v > L) break;
        a3397_p9[b] = v;
        a3397_np = b + 1;
    }
    a3397_len = 0;
    a3397_rec(8, 1, 0);
    qsort(a3397_buf, a3397_len, sizeof(uint64_t), a3397_cmp);
    size_t k = 0;
    for (size_t i = 0; i < a3397_len; i++) {
        if (k == 0 || a3397_buf[i] != a3397_buf[k - 1]) a3397_buf[k++] = a3397_buf[i];
    }
    free(a3397_terms);
    a3397_terms = (uint64_t *)malloc((k ? k : 1) * sizeof(uint64_t));
    for (size_t i = 0; i < k; i++) a3397_terms[i] = a3397_buf[i];
    a3397_cnt = k;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if ((uint64_t)n > a3397_cnt) {
        uint64_t L = a3397_L ? a3397_L : ((uint64_t)1 << 20);
        for (;;) {
            a3397_build(L);
            if ((uint64_t)n <= a3397_cnt) break;
            if (L > ((uint64_t)1 << 61)) break;
            L *= 2;
        }
    }
    if ((uint64_t)n > a3397_cnt) return 0;
    return (seq_t)a3397_terms[n - 1];
}
