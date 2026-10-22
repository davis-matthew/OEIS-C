/*
 * A003354  Numbers that are the sum of 9 positive 5th powers.
 * https://oeis.org/A003354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sieve. Bitset DP marks every number <= L that is a sum of
 *            exactly 9 positive 5th powers; L is doubled until at least n
 *            terms are found
 * Range:     u8 n <= 9, u16 n <= 5721, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a3354_shift_or
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/a3354_shift_or.h"

static uint64_t *a3354_terms = NULL;
static size_t a3354_nterms = 0;

static void a3354_build(uint64_t L) {
    size_t W = (size_t)(L / 64 + 1);
    uint64_t *cur = (uint64_t *)calloc(W, sizeof(uint64_t));
    uint64_t *nxt = (uint64_t *)calloc(W, sizeof(uint64_t));
    uint64_t pw[64];
    int nb = 0, k, t;
    uint64_t b, v;
    size_t cnt = 0;

    for (b = 1; ; b++) {
        uint64_t p = b * b * b * b * b;
        if (p > L) break;
        pw[nb++] = p;
    }
    cur[0] = 1;
    for (k = 1; k <= 9; k++) {
        memset(nxt, 0, W * sizeof(uint64_t));
        for (t = 0; t < nb; t++) a3354_shift_or(nxt, cur, W, pw[t]);
        { uint64_t *tmp = cur; cur = nxt; nxt = tmp; }
    }
    for (v = 0; v <= L; v++)
        if ((cur[v / 64] >> (v % 64)) & 1) cnt++;
    free(a3354_terms);
    a3354_terms = (uint64_t *)malloc((cnt + 1) * sizeof(uint64_t));
    a3354_nterms = 0;
    for (v = 0; v <= L; v++)
        if ((cur[v / 64] >> (v % 64)) & 1) a3354_terms[a3354_nterms++] = v;
    free(cur);
    free(nxt);
}

seq_t a(idx_t n) {
    static uint64_t L = 0;
    if (n < 1) n = 1;
    if (L == 0) { L = 4096; a3354_build(L); }
    while ((idx_t)a3354_nterms < n) {
        L *= 2;
        a3354_build(L);
    }
    return (seq_t)a3354_terms[n - 1];
}
