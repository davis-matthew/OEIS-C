/*
 * A004841  Numbers that are the sum of at most 12 nonzero 4th powers.
 * https://oeis.org/A004841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve -- a minimum-count DP c(m) = 1 + min_k c(m - k^4), c(0) =
 *            0, lists the m with c(m) <= 12 in increasing order
 * Range:     u8 n <= 175, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COUNT 12

static uint64_t *terms = NULL;
static uint64_t term_count = 0;
static uint64_t sieved_limit = 0;

static void sieve(uint64_t limit) {
    unsigned char *c = (unsigned char *)malloc((size_t)limit * sizeof(unsigned char));
    uint64_t m, k;
    memset(c, 255, (size_t)limit);
    c[0] = 0;
    for (m = 1; m < limit; m++) {
        unsigned char best = 255;
        for (k = 1; k * k * k * k <= m; k++) {
            unsigned char v = c[m - k * k * k * k];
            if (v < best) best = v;
        }
        if (best != 255) c[m] = (unsigned char)(best + 1 > 254 ? 254 : best + 1);
    }
    free(terms);
    terms = (uint64_t *)malloc((size_t)limit * sizeof(uint64_t));
    term_count = 0;
    for (m = 0; m < limit; m++) {
        if (c[m] <= MAX_COUNT) terms[term_count++] = m;
    }
    sieved_limit = limit;
    free(c);
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (term_count < n) {
        uint64_t limit = sieved_limit ? sieved_limit : 1024;
        while (limit < 2 * (uint64_t)n + 64) limit *= 2;
        sieve(limit);
        while (term_count < n) {
            limit *= 2;
            sieve(limit);
        }
    }
    return (seq_t)terms[n - 1];
}
