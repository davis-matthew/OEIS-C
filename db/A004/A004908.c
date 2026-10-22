/*
 * A004908  Numbers that are the sum of at most 2 positive 11th powers.
 * https://oeis.org/A004908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 36, u64 n <= 1631,
 *            u128 n <= 10000, big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: list all sums i^11 + j^11 (0 <= i <= j <= K) not exceeding K^11, sort, drop duplicates; K doubles until at least n terms are known, a(n) = n-th smallest */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/u128_to_seq.h"

typedef unsigned __int128 u128;

static u128 *vals = NULL;
static idx_t nvals = 0;
static int curK = 0;

static int cmp_u128(const void *a, const void *b) {
    u128 x = *(const u128 *)a, y = *(const u128 *)b;
    return x < y ? -1 : (x > y ? 1 : 0);
}

static void build(int K) {
    u128 *pw = (u128 *)malloc((size_t)(K + 1) * sizeof(u128));
    for (int m = 0; m <= K; m++) {
        u128 p = 1;
        for (int t = 0; t < 11; t++) p *= (u128)m;
        pw[m] = p;
    }
    u128 lim = pw[K];
    size_t cap = (size_t)(K + 1) * (size_t)(K + 2) / 2;
    u128 *v = (u128 *)malloc(cap * sizeof(u128));
    size_t cnt = 0;
    for (int i = 0; i <= K; i++) {
        if (pw[i] + pw[i] > lim) break;
        for (int j = i; j <= K; j++) {
            u128 s = pw[i] + pw[j];
            if (s > lim) break;
            v[cnt++] = s;
        }
    }
    qsort(v, cnt, sizeof(u128), cmp_u128);
    size_t d = 0;
    for (size_t k = 0; k < cnt; k++) {
        if (d == 0 || v[k] != v[d - 1]) v[d++] = v[k];
    }
    free(pw);
    free(vals);
    vals = v;
    nvals = (idx_t)d;
    curK = K;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    while (nvals < n && curK < 2900) {
        int K = curK ? curK * 2 : 64;
        if (K > 2900) K = 2900;
        build(K);
    }
    if (n > nvals) return (seq_t)0;
    return u128_to_seq(vals[n - 1]);
}
