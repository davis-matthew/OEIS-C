/*
 * A003366  Numbers that are the sum of 10 positive 6th powers.
 * https://oeis.org/A003366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1015, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   s3366_shor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: bitset DP of numbers that are sums of exactly 10 positive 6th powers, below a limit that grows until it covers a(n) */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/s3366_shor.h"

static uint64_t *s3366_terms = NULL;
static idx_t s3366_count = 0;
static uint64_t s3366_limit = 0;

static void s3366_build(uint64_t L)
{
    size_t W = (size_t)((L + 63) / 64);
    uint64_t *cur = (uint64_t *)calloc(W, sizeof(uint64_t));
    uint64_t *nxt = (uint64_t *)calloc(W, sizeof(uint64_t));
    uint64_t pw[64];
    int np = 0, k, j;
    uint64_t b, v;
    idx_t cnt = 0;

    for (b = 1; b < 64; b++) {
        uint64_t p = b * b * b;
        p = p * p;
        if (p >= L) break;
        pw[np++] = p;
    }
    cur[0] = 1;
    for (k = 0; k < 10; k++) {
        memset(nxt, 0, W * sizeof(uint64_t));
        for (j = 0; j < np; j++) s3366_shor(nxt, cur, W, pw[j]);
        { uint64_t *t = cur; cur = nxt; nxt = t; }
    }
    if (W * 64 > L) {
        unsigned extra = (unsigned)(W * 64 - L);
        cur[W - 1] &= (~(uint64_t)0) >> extra;
    }
    for (v = 0; v < (uint64_t)W; v++) {
        uint64_t w = cur[v];
        while (w) { cnt++; w &= w - 1; }
    }
    free(s3366_terms);
    s3366_terms = (uint64_t *)malloc((size_t)(cnt + 1) * sizeof(uint64_t));
    cnt = 0;
    for (v = 0; v < L; v++)
        if ((cur[v >> 6] >> (v & 63)) & 1) s3366_terms[cnt++] = v;
    s3366_count = cnt;
    s3366_limit = L;
    free(cur);
    free(nxt);
}

seq_t a(idx_t n)
{
    uint64_t L = s3366_limit ? s3366_limit : ((uint64_t)1 << 16);
    if (n < 1) return 0;
    if (s3366_limit == 0) s3366_build(L);
    while (s3366_count < n) {
        L *= 4;
        s3366_build(L);
    }
    return (seq_t)s3366_terms[n - 1];
}
