/*
 * A004896  Numbers that are the sum of at most 12 positive 9th powers.
 * https://oeis.org/A004896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve -- enumerate all sums of at most 12 positive 9th powers
 *            with bases <= K, keep those < (K+1)^9 (they are exactly the
 *            terms below that bound), sort, dedupe; increase K until at least
 *            n terms are known
 * Range:     u8 n <= 13, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *vals = NULL;
static idx_t vcnt = 0, vcap = 0;
static uint64_t pw[64];

static void push(uint64_t v) {
    if (vcnt == vcap) {
        vcap = vcap ? vcap * 2 : 1024;
        vals = (uint64_t *)realloc(vals, vcap * sizeof(uint64_t));
    }
    vals[vcnt++] = v;
}

static void rec(idx_t b, idx_t K, int left, uint64_t sum, uint64_t lim) {
    push(sum);
    if (left == 0) return;
    for (idx_t i = b; i <= K; i++) {
        uint64_t s = sum + pw[i];
        if (s >= lim) break;
        rec(i, K, left - 1, s, lim);
    }
}

static int cmp_u64(const void *x, const void *y) {
    uint64_t p = *(const uint64_t *)x, q = *(const uint64_t *)y;
    return p < q ? -1 : (p > q ? 1 : 0);
}

static idx_t curK = 0;

static void build(idx_t K) {
    for (idx_t i = 1; i <= K + 1; i++) {
        uint64_t p = 1;
        for (int j = 0; j < 9; j++) p *= (uint64_t)i;
        pw[i] = p;
    }
    vcnt = 0;
    rec(1, K, 12, 0, pw[K + 1]);
    qsort(vals, vcnt, sizeof(uint64_t), cmp_u64);
    idx_t m = 0;
    for (idx_t i = 0; i < vcnt; i++)
        if (m == 0 || vals[m - 1] != vals[i]) vals[m++] = vals[i];
    vcnt = m;
    curK = K;
}

seq_t a(idx_t n) {
    if (curK == 0) build(1);
    while (vcnt < n && curK < 60) build(curK + 1);
    return (seq_t)vals[n - 1];
}
