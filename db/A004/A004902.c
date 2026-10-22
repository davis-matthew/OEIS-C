/*
 * A004902  Numbers that are the sum of at most 7 nonzero 10th powers.
 * https://oeis.org/A004902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve/search -- enumerate all sums of at most 7 nonzero 10th
 *            powers up to a bound X (doubling X until enough terms), sort,
 *            dedupe; a(n) = n-th smallest (a(1) = 0)
 * Range:     u8 n <= 8, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/u128_to_seq.h"

static uint64_t *vals = NULL;
static size_t nvals = 0, capvals = 0;
static uint64_t pw[80];
static int npw = 0;
static uint64_t curX = 0;

static int cmp_u64(const void *a, const void *b) {
    uint64_t x = *(const uint64_t *)a, y = *(const uint64_t *)b;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

static void push_val(uint64_t v) {
    if (nvals == capvals) {
        size_t nc = capvals ? capvals * 2 : 1024;
        vals = (uint64_t *)realloc(vals, nc * sizeof(uint64_t));
        capvals = nc;
    }
    vals[nvals++] = v;
}

static void dfs(int start, int depth, uint64_t sum) {
    push_val(sum);
    if (depth == 7) return;
    for (int i = start; i < npw && sum + pw[i] <= curX; i++)
        dfs(i, depth + 1, sum + pw[i]);
}

static void build(uint64_t X) {
    curX = X;
    npw = 0;
    for (uint64_t b = 1; b < 80; b++) {
        uint64_t p = 1;
        int ok = 1;
        for (int k = 0; k < 10; k++) {
            if (p > X / b) { ok = 0; break; }
            p *= b;
        }
        if (!ok || p > X) break;
        pw[npw++] = p;
    }
    nvals = 0;
    dfs(0, 0, 0);
    qsort(vals, nvals, sizeof(uint64_t), cmp_u64);
    size_t u = 0;
    for (size_t i = 0; i < nvals; i++)
        if (u == 0 || vals[i] != vals[u - 1]) vals[u++] = vals[i];
    nvals = u;
}

seq_t a(idx_t n) {
    if (curX == 0) build((uint64_t)1 << 20);
    while ((idx_t)nvals < n && curX < ((uint64_t)1 << 60))
        build(curX * 4);
    if (n == 0 || (idx_t)nvals < n) return 0;
    return u128_to_seq((unsigned __int128)vals[n - 1]);
}
