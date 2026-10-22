/*
 * A003341  Numbers that are the sum of 7 positive 4th powers.
 * https://oeis.org/A003341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sieve -- a number is a term iff it is a sum of exactly 7
 *            positive 4th powers; computed by an unbounded-knapsack bitmask
 *            DP over counts 0..7
 * Range:     u8 n <= 22, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static uint64_t *a003341_terms = NULL;
static size_t a003341_nterms = 0;

static void a003341_build(uint64_t L) {
    unsigned char *mask = (unsigned char *)calloc((size_t)L + 1, 1);
    uint64_t b, x;
    size_t cnt = 0;
    mask[0] = 1;
    for (b = 1; b * b * b * b <= L; b++) {
        uint64_t p = b * b * b * b;
        for (x = p; x <= L; x++)
            mask[x] |= (unsigned char)((mask[x - p] << 1) & 0xFF);
    }
    for (x = 0; x <= L; x++)
        if (mask[x] & 0x80) cnt++;
    free(a003341_terms);
    a003341_terms = (uint64_t *)malloc((cnt + 1) * sizeof(uint64_t));
    a003341_nterms = 0;
    for (x = 0; x <= L; x++)
        if (mask[x] & 0x80) a003341_terms[a003341_nterms++] = x;
    free(mask);
}

seq_t a(idx_t n) {
    static uint64_t limit = 0;
    if (n < 1) return 0;
    while (a003341_nterms < n) {
        limit = limit ? limit * 2 : (1u << 16);
        a003341_build(limit);
    }
    return (seq_t)a003341_terms[n - 1];
}
