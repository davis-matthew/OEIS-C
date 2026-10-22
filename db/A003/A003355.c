/*
 * A003355  Numbers that are the sum of 10 positive 5th powers.
 * https://oeis.org/A003355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th number that is a sum of exactly 10 positive 5th
 *            powers; computed by a bitset sieve over sums, layer by layer,
 *            doubling the bound as needed
 * Range:     u8 n <= 9, u16 n <= 8283, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a3355_shor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a3355_shor.h"

static uint64_t *a3355_terms = NULL;
static idx_t a3355_cnt = 0;

static void a3355_build(uint64_t L) {
    size_t W = (size_t)(L / 64 + 1);
    uint64_t *prev = (uint64_t *)calloc(W, sizeof(uint64_t));
    uint64_t *cur = (uint64_t *)calloc(W, sizeof(uint64_t));
    uint64_t pw[64];
    int np = 0, k, t;
    size_t i;
    uint64_t total = (uint64_t)W * 64;
    for (uint64_t p = 1;; p++) {
        uint64_t v = p * p * p * p * p;
        if (v >= total) break;
        pw[np++] = v;
    }
    prev[0] = 1;
    for (k = 1; k <= 10; k++) {
        memset(cur, 0, W * sizeof(uint64_t));
        for (t = 0; t < np; t++) a3355_shor(cur, prev, W, pw[t]);
        { uint64_t *tmp = prev; prev = cur; cur = tmp; }
    }
    free(a3355_terms);
    a3355_cnt = 0;
    {
        idx_t c = 0;
        for (i = 0; i < W; i++) {
            uint64_t w = prev[i];
            while (w) { c++; w &= w - 1; }
        }
        a3355_terms = (uint64_t *)malloc((c ? c : 1) * sizeof(uint64_t));
        for (i = 0; i < W; i++) {
            uint64_t w = prev[i];
            while (w) {
                int bit = __builtin_ctzll(w);
                a3355_terms[a3355_cnt++] = (uint64_t)i * 64 + (uint64_t)bit;
                w &= w - 1;
            }
        }
    }
    free(prev);
    free(cur);
}

seq_t a(idx_t n) {
    static uint64_t L = 0;
    if (n < 1) return 0;
    if (L == 0) { L = (uint64_t)1 << 16; a3355_build(L); }
    while (a3355_cnt < n) {
        L *= 2;
        a3355_build(L);
    }
    return (seq_t)a3355_terms[n - 1];
}
