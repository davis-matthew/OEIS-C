/*
 * A003364  Numbers that are the sum of 8 positive 6th powers.
 * https://oeis.org/A003364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sieve/DP — for x up to a growing limit L, track a bitmask of the
 *            counts k for which x is a sum of exactly k positive 6th powers
 *            (unbounded knapsack over b^6). a(n) is the n-th x whose mask
 *            includes k = 8
 * Range:     u8 n <= 4, u16 n <= 573, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *A3364_terms = NULL;
static idx_t A3364_cnt = 0;

static void A3364_build(uint64_t L) {
    uint16_t *mask = (uint16_t *)calloc((size_t)(L + 1), sizeof(uint16_t));
    uint64_t b, x, p;
    idx_t cnt = 0;
    mask[0] = 1;
    for (b = 1;; b++) {
        p = b * b * b;
        p = p * p;
        if (p > L) break;
        for (x = p; x <= L; x++)
            mask[x] |= (uint16_t)((mask[x - p] << 1) & 0x1FF);
    }
    for (x = 1; x <= L; x++)
        if (mask[x] & 0x100) cnt++;
    free(A3364_terms);
    A3364_terms = (uint64_t *)malloc((size_t)(cnt + 1) * sizeof(uint64_t));
    cnt = 0;
    for (x = 1; x <= L; x++)
        if (mask[x] & 0x100) A3364_terms[cnt++] = x;
    A3364_cnt = cnt;
    free(mask);
}

seq_t a(idx_t n) {
    uint64_t L = 1 << 16;
    if (n == 0) return 0;
    while (A3364_cnt < n) {
        A3364_build(L);
        L *= 2;
    }
    return (seq_t)A3364_terms[n - 1];
}
