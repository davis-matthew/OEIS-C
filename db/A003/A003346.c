/*
 * A003346  Numbers that are the sum of 12 positive 4th powers.
 * https://oeis.org/A003346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sieve, not a closed form. S_0 = {0}, S_k = { s + p^4 : s in
 *            S_{k-1}, p >= 1 }. a(n) is the n-th smallest element of S_12.
 *            The bitsets are rebuilt with a doubled limit until they contain
 *            at least n terms
 * Range:     u8 n <= 31, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a3346_shift_or
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a3346_shift_or.h"

static uint64_t *a3346_terms = NULL;
static idx_t a3346_cnt = 0;

static void a3346_build(uint64_t L)
{
    size_t W = (size_t)(L / 64 + 1);
    uint64_t *cur = (uint64_t *)calloc(W, sizeof(uint64_t));
    uint64_t *nxt = (uint64_t *)calloc(W, sizeof(uint64_t));
    uint64_t maxbit = (uint64_t)W * 64;
    int k;
    uint64_t m, cnt = 0;
    cur[0] = 1;
    for (k = 1; k <= 12; k++) {
        uint64_t p;
        memset(nxt, 0, W * sizeof(uint64_t));
        for (p = 1; p * p * p * p < maxbit; p++)
            a3346_shift_or(nxt, cur, W, p * p * p * p);
        { uint64_t *t = cur; cur = nxt; nxt = t; }
    }
    for (m = 0; m <= L; m++)
        if ((cur[m / 64] >> (m % 64)) & 1) cnt++;
    free(a3346_terms);
    a3346_terms = (uint64_t *)malloc((size_t)(cnt ? cnt : 1) * sizeof(uint64_t));
    a3346_cnt = 0;
    for (m = 0; m <= L; m++)
        if ((cur[m / 64] >> (m % 64)) & 1) a3346_terms[a3346_cnt++] = m;
    free(cur);
    free(nxt);
}

seq_t a(idx_t n)
{
    if (n > a3346_cnt) {
        uint64_t L = 1u << 16;
        while (L / 64 < (uint64_t)n) L <<= 1;
        for (;;) {
            a3346_build(L);
            if (a3346_cnt >= n) break;
            L <<= 1;
        }
    }
    return (seq_t)a3346_terms[n - 1];
}
