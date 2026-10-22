/*
 * A003377  Numbers that are the sum of 10 positive 7th powers.
 * https://oeis.org/A003377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 202, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sieve: mark every s <= L that is a sum of exactly 10 positive 7th powers
   (unbounded knapsack with a bitmask of how many parts were used), list
   them in increasing order, and double L until there are at least n terms. */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *A3377_terms = 0;
static size_t A3377_cnt = 0;
static uint64_t A3377_lim = 0;

static void A3377_build(uint64_t L) {
    uint16_t *m = (uint16_t *)calloc((size_t)(L + 1), sizeof(uint16_t));
    uint64_t b, s, p;
    size_t c = 0;
    if (!m) return;
    m[0] = 1;
    for (b = 1; ; b++) {
        p = b * b * b * b * b * b * b;
        if (p > L) break;
        for (s = p; s <= L; s++)
            m[s] |= (uint16_t)((m[s - p] << 1) & 0x7FF);
    }
    for (s = 0; s <= L; s++)
        if (m[s] & 0x400) c++;
    free(A3377_terms);
    A3377_terms = (uint64_t *)malloc((c ? c : 1) * sizeof(uint64_t));
    A3377_cnt = 0;
    if (A3377_terms) {
        for (s = 0; s <= L; s++)
            if (m[s] & 0x400) A3377_terms[A3377_cnt++] = s;
    }
    A3377_lim = L;
    free(m);
}

seq_t a(idx_t n) {
    while (A3377_cnt < (size_t)n) {
        uint64_t L = A3377_lim ? A3377_lim * 2 : ((uint64_t)1 << 16);
        A3377_build(L);
        if (!A3377_terms) return 0;
    }
    return (seq_t)A3377_terms[n - 1];
}
