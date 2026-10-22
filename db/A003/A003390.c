/*
 * A003390  Sum of 12 nonzero 8th powers.
 * https://oeis.org/A003390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sieve -- a bitset DP marks every number <= L that is a sum of
 *            exactly 12 positive 8th powers; L doubles until at least n terms
 *            are found, and a(n) is the n-th marked number
 * Range:     u8 n <= 1, u16 n <= 85, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   A3390_shor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/A3390_shor.h"

static uint64_t *A3390_terms = NULL;
static idx_t A3390_cnt = 0;

static void A3390_build(uint64_t L) {
    size_t W = (size_t)(L / 64 + 1);
    uint64_t *bs[13];
    for (int k = 0; k <= 12; k++)
        bs[k] = (uint64_t *)calloc(W, sizeof(uint64_t));
    bs[0][0] = 1;
    for (uint64_t b = 1;; b++) {
        uint64_t p = b * b; p *= p; p *= p;
        if (p > L) break;
        for (int k = 1; k <= 12; k++)
            A3390_shor(bs[k], bs[k - 1], W, p);
    }
    idx_t cnt = 0;
    for (uint64_t v = 0; v <= L; v++)
        if ((bs[12][v / 64] >> (v % 64)) & 1) cnt++;
    free(A3390_terms);
    A3390_terms = (uint64_t *)malloc((size_t)(cnt ? cnt : 1) * sizeof(uint64_t));
    idx_t c = 0;
    for (uint64_t v = 0; v <= L; v++)
        if ((bs[12][v / 64] >> (v % 64)) & 1) A3390_terms[c++] = v;
    A3390_cnt = cnt;
    for (int k = 0; k <= 12; k++) free(bs[k]);
}

seq_t a(idx_t n) {
    static uint64_t L = 0;
    if (L == 0) { L = (uint64_t)1 << 16; A3390_build(L); }
    while (A3390_cnt < n) {
        L *= 2;
        A3390_build(L);
    }
    return (seq_t)A3390_terms[n - 1];
}
