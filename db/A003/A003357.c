/*
 * A003357  Numbers that are the sum of 12 positive 5th powers.
 * https://oeis.org/A003357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sieve. Each s <= L gets a bitmask of the counts k for which s is
 *            a sum of k positive 5th powers (unbounded knapsack over the 5th
 *            powers). Terms are the s with bit 12 set; L doubles until there
 *            are n terms
 * Range:     u8 n <= 9, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *terms = NULL;
static idx_t nterms = 0;

static void build(uint64_t L) {
    uint16_t *mask = (uint16_t *)malloc((size_t)(L + 1) * sizeof(uint16_t));
    uint64_t b, s, cnt;
    memset(mask, 0, (size_t)(L + 1) * sizeof(uint16_t));
    mask[0] = 1;
    for (b = 1;; b++) {
        uint64_t p = b * b * b * b * b;
        if (p > L) break;
        for (s = p; s <= L; s++)
            mask[s] |= (uint16_t)((mask[s - p] << 1) & 0x1FFF);
    }
    cnt = 0;
    for (s = 1; s <= L; s++)
        if (mask[s] & (1u << 12)) cnt++;
    free(terms);
    terms = (uint64_t *)malloc((size_t)(cnt ? cnt : 1) * sizeof(uint64_t));
    nterms = 0;
    for (s = 1; s <= L; s++)
        if (mask[s] & (1u << 12)) terms[nterms++] = s;
    free(mask);
}

seq_t a(idx_t n) {
    static uint64_t L = 0;
    if (n < 1) return 0;
    while (nterms < n) {
        L = L ? L * 2 : (1u << 16);
        build(L);
    }
    return (seq_t)terms[n - 1];
}
