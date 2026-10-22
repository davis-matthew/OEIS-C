/*
 * A004901  Numbers that are the sum of at most 6 nonzero 10th powers.
 * https://oeis.org/A004901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve/search: enumerate all sums of at most 6 nonzero 10th
 *            powers <= K^10 (bases <= K), sort, dedupe; enlarge K until n
 *            terms are found
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/u128_to_seq.h"

static uint64_t *vals = NULL;
static size_t nvals = 0;
static size_t capv = 0;
static int curK = 0;
static uint64_t pw[80];
static uint64_t lim;
static int maxb;

static int cmp_u64(const void *x, const void *y) {
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u < v) ? -1 : (u > v) ? 1 : 0;
}

static void push(uint64_t s) {
    if (nvals == capv) {
        capv = capv ? capv * 2 : 1024;
        vals = (uint64_t *)realloc(vals, capv * sizeof(uint64_t));
    }
    vals[nvals++] = s;
}

static void dfs(int start, int depth, uint64_t s) {
    int b;
    push(s);
    if (depth == 6) return;
    for (b = start; b <= maxb; b++) {
        uint64_t t = s + pw[b];
        if (t > lim) break;
        dfs(b, depth + 1, t);
    }
}

static void build(int K) {
    int b, i;
    size_t j, u;
    for (b = 0; b <= K; b++) {
        uint64_t p = 1;
        for (i = 0; i < 10; i++) p *= (uint64_t)b;
        pw[b] = p;
    }
    maxb = K;
    lim = pw[K];
    nvals = 0;
    dfs(1, 0, 0);
    qsort(vals, nvals, sizeof(uint64_t), cmp_u64);
    u = 0;
    for (j = 0; j < nvals; j++) {
        if (u == 0 || vals[j] != vals[u - 1]) vals[u++] = vals[j];
    }
    nvals = u;
    curK = K;
}

seq_t a(idx_t n) {
    while (nvals < n && curK < 70) {
        int K = curK ? curK + 3 : 4;
        if (K > 70) K = 70;
        build(K);
    }
    if (n == 0 || n > nvals) return u128_to_seq(0);
    return u128_to_seq((unsigned __int128)vals[n - 1]);
}
