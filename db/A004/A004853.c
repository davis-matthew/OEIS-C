/*
 * A004853  Numbers that are the sum of at most 2 nonzero 6th powers.
 * https://oeis.org/A004853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search: generate all j^6 + k^6 with 0 <= j <= k <= K, sort,
 *            dedupe, keep terms < (K+1)^6 (complete), enlarge K as needed
 * Range:     u8 n <= 6, u16 n <= 27, u32 n <= 826, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/u128_to_seq.h"

typedef unsigned __int128 u128_t;

static u128_t *memo = NULL;
static idx_t memo_len = 0;

static int cmp_u128(const void *pa, const void *pb) {
    u128_t x = *(const u128_t *)pa, y = *(const u128_t *)pb;
    return (x > y) - (x < y);
}

static u128_t p6(idx_t k) {
    u128_t t = (u128_t)k * k * k;
    return t * t;
}

static void build(idx_t K) {
    idx_t total = (K + 1) * (K + 2) / 2;
    u128_t *v = (u128_t *)malloc(total * sizeof(u128_t));
    u128_t *pw = (u128_t *)malloc((K + 1) * sizeof(u128_t));
    idx_t i, j, k, cnt = 0, m = 0;
    u128_t lim = p6(K + 1);
    for (i = 0; i <= K; i++) pw[i] = p6(i);
    for (k = 0; k <= K; k++)
        for (j = 0; j <= k; j++) {
            u128_t s = pw[j] + pw[k];
            if (s < lim) v[cnt++] = s;
        }
    qsort(v, cnt, sizeof(u128_t), cmp_u128);
    for (i = 0; i < cnt; i++)
        if (m == 0 || v[i] != v[m - 1]) v[m++] = v[i];
    free(pw);
    free(memo);
    memo = v;
    memo_len = m;
}

seq_t a(idx_t n) {
    if (n > memo_len) {
        idx_t K = 2;
        while (K * K < 3 * n) K++;
        K += 4;
        for (;;) {
            build(K);
            if (memo_len >= n) break;
            K += K / 2 + 1;
        }
    }
    return u128_to_seq(memo[n - 1]);
}
