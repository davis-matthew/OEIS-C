/*
 * A003379  Numbers that are the sum of 12 positive 7th powers.
 * https://oeis.org/A003379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sieve -- a bitset DP over k = 1..12 marks every sum of k
 *            positive 7th powers below a limit; the limit doubles until it
 *            holds at least n terms, and a(n) is the n-th marked number
 * Range:     u8 n <= 2, u16 n <= 287, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a3379_shor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/a3379_shor.h"

static uint64_t *a3379_terms = NULL;
static size_t a3379_cnt = 0;

static void a3379_build(uint64_t L) {
    size_t W = (size_t)((L + 63) / 64);
    uint64_t *cur = (uint64_t *)calloc(W, sizeof(uint64_t));
    uint64_t *nxt = (uint64_t *)calloc(W, sizeof(uint64_t));
    int k;
    uint64_t x, cnt = 0;
    cur[0] = 1;
    for (k = 0; k < 12; k++) {
        uint64_t b;
        memset(nxt, 0, W * sizeof(uint64_t));
        for (b = 1; ; b++) {
            uint64_t p = b * b * b * b * b * b * b;
            if (p >= L) break;
            a3379_shor(nxt, cur, W, p);
        }
        { uint64_t *t = cur; cur = nxt; nxt = t; }
    }
    for (x = 0; x < L; x++)
        if ((cur[x >> 6] >> (x & 63)) & 1) cnt++;
    free(a3379_terms);
    a3379_terms = (uint64_t *)malloc((size_t)(cnt ? cnt : 1) * sizeof(uint64_t));
    a3379_cnt = 0;
    for (x = 0; x < L; x++)
        if ((cur[x >> 6] >> (x & 63)) & 1) a3379_terms[a3379_cnt++] = x;
    free(cur);
    free(nxt);
}

seq_t a(idx_t n) {
    static uint64_t limit = 0;
    if (limit == 0) {
        limit = (uint64_t)1 << 16;
        a3379_build(limit);
    }
    while ((idx_t)a3379_cnt < n) {
        limit *= 2;
        a3379_build(limit);
    }
    return (seq_t)a3379_terms[n - 1];
}
