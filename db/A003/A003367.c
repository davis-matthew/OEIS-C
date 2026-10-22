/*
 * A003367  Numbers that are the sum of 11 positive 6th powers.
 * https://oeis.org/A003367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve. A bitset DP finds which numbers < L are sums of exactly
 *            11 positive 6th powers (L doubles until there are n terms); a(n)
 *            is the n-th such number in increasing order
 * Range:     u8 n <= 4, u16 n <= 1291, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a3367_shor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/a3367_shor.h"

static uint64_t *terms_cache = NULL;
static idx_t terms_count = 0;
static uint64_t terms_limit = 0;

static void a3367_build(uint64_t L)
{
    size_t W = (size_t)(L / 64);
    uint64_t *x = (uint64_t *)calloc(W, sizeof(uint64_t));
    uint64_t *y = (uint64_t *)calloc(W, sizeof(uint64_t));
    uint64_t pw[64];
    int np = 0, k, i;
    uint64_t b;
    size_t w, cnt = 0, pos = 0;

    for (b = 1;; b++) {
        uint64_t p = b * b * b;
        p = p * p;
        if (p >= L) break;
        pw[np++] = p;
    }
    x[0] = 1;
    for (k = 0; k < 11; k++) {
        uint64_t *t;
        memset(y, 0, W * sizeof(uint64_t));
        for (i = 0; i < np; i++) a3367_shor(y, x, W, pw[i]);
        t = x; x = y; y = t;
    }
    for (w = 0; w < W; w++) cnt += (size_t)__builtin_popcountll(x[w]);
    free(terms_cache);
    terms_cache = (uint64_t *)malloc((cnt + 1) * sizeof(uint64_t));
    for (w = 0; w < W; w++) {
        uint64_t v = x[w];
        while (v) {
            int t = __builtin_ctzll(v);
            terms_cache[pos++] = (uint64_t)w * 64 + (uint64_t)t;
            v &= v - 1;
        }
    }
    terms_count = (idx_t)cnt;
    terms_limit = L;
    free(x);
    free(y);
}

seq_t a(idx_t n)
{
    while (terms_count < n) {
        uint64_t L = terms_limit ? terms_limit * 2 : ((uint64_t)1 << 20);
        a3367_build(L);
    }
    return (seq_t)terms_cache[n - 1];
}
