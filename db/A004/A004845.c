/*
 * A004845  Numbers that are the sum of at most 5 positive 5th powers.
 * https://oeis.org/A004845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- enumerate every multiset of at most 5 positive 5th
 *            powers whose sum is <= L, sort and remove duplicates; double L
 *            until at least n distinct values are found
 * Range:     u8 n <= 26, u16 n <= 1176, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/u128_to_seq.h"

static uint64_t *vals = NULL;
static size_t nvals = 0;
static size_t vcap = 0;
static uint64_t lim = 0;
static uint64_t pw[4096];
static size_t npw = 0;

static int cmp_u64(const void *x, const void *y) {
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static void push(uint64_t v) {
    if (nvals == vcap) {
        vcap = vcap ? vcap * 2 : 1024;
        vals = (uint64_t *)realloc(vals, vcap * sizeof(uint64_t));
    }
    vals[nvals++] = v;
}

static void gen(size_t start, int depth, uint64_t sum) {
    size_t b;
    push(sum);
    if (depth == 5) return;
    for (b = start; b < npw && sum + pw[b] <= lim; b++)
        gen(b, depth + 1, sum + pw[b]);
}

static void build(void) {
    uint64_t b;
    size_t i, k;
    npw = 0;
    for (b = 1; b < 4096; b++) {
        uint64_t p = b * b * b * b * b;
        if (p > lim) break;
        pw[npw++] = p;
    }
    nvals = 0;
    gen(0, 0, 0);
    qsort(vals, nvals, sizeof(uint64_t), cmp_u64);
    k = 0;
    for (i = 0; i < nvals; i++)
        if (k == 0 || vals[i] != vals[k - 1]) vals[k++] = vals[i];
    nvals = k;
}

seq_t a(idx_t n) {
    while (vals == NULL || (idx_t)nvals < n) {
        lim = lim ? lim * 2 : ((uint64_t)1 << 16);
        build();
    }
    return u128_to_seq((unsigned __int128)vals[n - 1]);
}
