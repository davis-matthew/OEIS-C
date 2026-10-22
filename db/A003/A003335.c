/*
 * A003335  Numbers that are the sum of 12 positive cubes.
 * https://oeis.org/A003335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sieve. mask[m] has bit k set when m is a sum of exactly k
 *            positive cubes: mask[0] = 1, and for each cube c, mask[m] |=
 *            mask[m-c] << 1 (unbounded knapsack). a(n) is the n-th m whose
 *            mask has bit 12 set
 * Range:     u8 n <= 166, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *terms = 0;
static idx_t nterms = 0;
static uint64_t sieved = 0;

static void build(uint64_t L) {
    uint16_t *mask = (uint16_t *)calloc((size_t)(L + 1), sizeof(uint16_t));
    uint64_t m, c, b;
    idx_t cnt = 0;
    mask[0] = 1;
    for (b = 1; (c = b * b * b) <= L; b++) {
        for (m = c; m <= L; m++) {
            mask[m] |= (uint16_t)((mask[m - c] << 1) & 0x1FFF);
        }
    }
    for (m = 1; m <= L; m++)
        if (mask[m] & (1u << 12)) cnt++;
    free(terms);
    terms = (uint64_t *)malloc((size_t)(cnt + 1) * sizeof(uint64_t));
    nterms = 0;
    for (m = 1; m <= L; m++)
        if (mask[m] & (1u << 12)) terms[nterms++] = m;
    free(mask);
    sieved = L;
}

seq_t a(idx_t n) {
    uint64_t L = sieved ? sieved : 1024;
    while (nterms < n) {
        if (sieved) L *= 2;
        build(L);
    }
    return (seq_t)terms[n - 1];
}
