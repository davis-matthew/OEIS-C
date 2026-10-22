/*
 * A004914  Numbers that are the sum of at most 8 positive 11th powers.
 * https://oeis.org/A004914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- enumerate all sums of at most 8 positive 11th powers
 *            with bases 1..K below (K+1)^11 (that range is then complete),
 *            sort, dedupe, and increase K until there are at least n terms;
 *            a(n) is the n-th smallest
 * Range:     u8 n <= 9, u16 n <= 45, u32 n <= 5180, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/u128_to_seq.h"

static uint64_t *vals = NULL;
static size_t nvals = 0;
static int curK = 0;

static uint64_t *buf = NULL;
static size_t bufLen = 0, bufCap = 0;

static void push_val(uint64_t v) {
    if (bufLen == bufCap) {
        bufCap = bufCap ? bufCap * 2 : 1024;
        buf = (uint64_t *)realloc(buf, bufCap * sizeof(uint64_t));
    }
    buf[bufLen++] = v;
}

static void gen(int left, int minb, int K, uint64_t s, const uint64_t *pw, uint64_t lim) {
    int b;
    push_val(s);
    if (left == 0) return;
    for (b = minb; b <= K; b++) {
        uint64_t t = s + pw[b];
        if (t >= lim) break;
        gen(left - 1, b, K, t, pw, lim);
    }
}

static int cmp_u64(const void *x, const void *y) {
    uint64_t p = *(const uint64_t *)x, q = *(const uint64_t *)y;
    return (p > q) - (p < q);
}

static void build(int K) {
    uint64_t pw[64];
    uint64_t lim;
    size_t i, m;
    int b, j;
    for (b = 0; b <= K + 1; b++) {
        uint64_t p = 1;
        for (j = 0; j < 11; j++) p *= (uint64_t)b;
        pw[b] = p;
    }
    lim = pw[K + 1];
    bufLen = 0;
    gen(8, 1, K, 0, pw, lim);
    qsort(buf, bufLen, sizeof(uint64_t), cmp_u64);
    m = 0;
    for (i = 0; i < bufLen; i++)
        if (m == 0 || buf[i] != buf[m - 1]) buf[m++] = buf[i];
    free(vals);
    vals = (uint64_t *)malloc((m ? m : 1) * sizeof(uint64_t));
    for (i = 0; i < m; i++) vals[i] = buf[i];
    nvals = m;
    curK = K;
}

seq_t a(idx_t n) {
    int K = curK ? curK : 1;
    while (nvals < n && K < 54) {
        if (K != curK || vals == NULL) build(K);
        if (nvals >= n) break;
        K++;
    }
    return u128_to_seq((unsigned __int128)vals[n - 1]);
}
