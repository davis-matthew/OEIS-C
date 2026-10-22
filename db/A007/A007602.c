/*
 * A007602  Numbers that are divisible by the product of their digits.
 * https://oeis.org/A007602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   search. a(n) = n-th number whose product of digits divides it.
 *            For each digit count, go through every multiset of nonzero
 *            digits. Rule out a multiset if it has a 5 and an even digit, or
 *            if its digit sum fails the divisibility test for the 3 or 9 in
 *            the product. Build each permutation from the las...
 * Range:     u8 n <= 25, u16 n <= 173, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   z_cmp, z_push
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/z_cmp.h"
#include "../utils/z_push.h"

static uint64_t *z_terms = NULL;
static size_t z_nterms = 0, z_cap = 0;
static uint64_t *z_buf = NULL;
static size_t z_nbuf = 0, z_bcap = 0;
static int z_len = 0;          /* lengths 1..z_len already generated */
static int z_cnt[10];
static int z_L, z_A, z_C;
static uint64_t z_P;
static uint64_t z_pow5[21];

static void z_dfs(int j, uint64_t suf, uint64_t p10) {
    int d;
    if (j == z_L) {
        if (suf % z_P == 0) z_push(&z_buf, &z_nbuf, &z_bcap, suf);
        return;
    }
    for (d = 1; d <= 9; d++) {
        uint64_t s;
        int k;
        if (!z_cnt[d]) continue;
        s = suf + (uint64_t)d * p10;
        k = j + 1;
        if (z_A) {
            int e = k < z_A ? k : z_A;
            if (s & ((1ULL << e) - 1)) continue;
        }
        if (z_C) {
            int e = k < z_C ? k : z_C;
            if (s % z_pow5[e]) continue;
        }
        z_cnt[d]--;
        z_dfs(k, s, p10 * 10);
        z_cnt[d]++;
    }
}

static void z_process(void) {
    int d, i, b, sum;
    bool has5 = z_cnt[5] > 0;
    bool haseven = (z_cnt[2] + z_cnt[4] + z_cnt[6] + z_cnt[8]) > 0;
    if (has5 && haseven) return;
    z_P = 1;
    sum = 0;
    for (d = 1; d <= 9; d++) {
        for (i = 0; i < z_cnt[d]; i++) z_P *= (uint64_t)d;
        sum += d * z_cnt[d];
    }
    z_A = z_cnt[2] + 2 * z_cnt[4] + z_cnt[6] + 3 * z_cnt[8];
    z_C = z_cnt[5];
    b = z_cnt[3] + z_cnt[6] + 2 * z_cnt[9];
    if (b >= 1 && sum % 3 != 0) return;
    if (b >= 2 && sum % 9 != 0) return;
    z_dfs(0, 0, 1);
}

static void z_ms(int d, int rem) {
    int c;
    if (d == 9) {
        z_cnt[9] = rem;
        z_process();
        return;
    }
    for (c = 0; c <= rem; c++) {
        z_cnt[d] = c;
        z_ms(d + 1, rem - c);
    }
    z_cnt[d] = 0;
}

static void z_gen_next(void) {
    size_t i;
    int d;
    z_L = z_len + 1;
    for (d = 0; d < 10; d++) z_cnt[d] = 0;
    z_nbuf = 0;
    z_ms(1, z_L);
    qsort(z_buf, z_nbuf, sizeof(uint64_t), z_cmp);
    for (i = 0; i < z_nbuf; i++) z_push(&z_terms, &z_nterms, &z_cap, z_buf[i]);
    z_len = z_L;
}

seq_t a(idx_t n) {
    if (z_pow5[0] == 0) {
        int i;
        z_pow5[0] = 1;
        for (i = 1; i < 21; i++) z_pow5[i] = z_pow5[i - 1] * 5;
    }
    while ((idx_t)z_nterms < n && z_len < 19) z_gen_next();
    if (n == 0 || (idx_t)z_nterms < n) return (seq_t)0;
    return (seq_t)z_terms[n - 1];
}
