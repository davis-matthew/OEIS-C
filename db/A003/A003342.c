/*
 * A003342  Numbers that are the sum of 8 positive 4th powers.
 * https://oeis.org/A003342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sieve. Keep a bitmask per value v whose bit k says v is a sum of
 *            k positive 4th powers (unbounded knapsack over 4th powers, k <=
 *            8). Values with bit 8 set are the terms; the bound doubles until
 *            there are n of them
 * Range:     u8 n <= 24, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *a3342_terms = 0;
static idx_t a3342_nterms = 0;
static uint64_t a3342_lim = 0;

static void a3342_build(uint64_t L) {
    uint16_t *mask = (uint16_t *)calloc((size_t)(L + 1), sizeof(uint16_t));
    uint64_t b, v, p;
    idx_t cnt = 0;
    if (!mask) return;
    mask[0] = 1;
    for (b = 1; (p = b * b * b * b) <= L; b++) {
        for (v = p; v <= L; v++) {
            mask[v] |= (uint16_t)((mask[v - p] << 1) & 0x1FF);
        }
    }
    for (v = 0; v <= L; v++)
        if (mask[v] & 0x100) cnt++;
    free(a3342_terms);
    a3342_terms = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)(cnt + 1));
    a3342_nterms = 0;
    if (a3342_terms) {
        for (v = 0; v <= L; v++)
            if (mask[v] & 0x100) a3342_terms[a3342_nterms++] = v;
    }
    a3342_lim = L;
    free(mask);
}

seq_t a(idx_t n) {
    uint64_t L;
    if (n < 1) return 0;
    while (a3342_nterms < n) {
        L = a3342_lim ? a3342_lim * 2 : 4096;
        a3342_build(L);
        if (!a3342_terms) return 0;
    }
    return (seq_t)a3342_terms[n - 1];
}
