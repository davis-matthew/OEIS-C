/*
 * A004851  Numbers that are the sum of at most 11 positive 5th powers.
 * https://oeis.org/A004851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve by min-coin DP: c(m) = min over k>=1, k^5<=m of
 *            c(m-k^5)+1, c(0)=0; terms are m with c(m) <= 11, in increasing
 *            order
 * Range:     u8 n <= 79, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *terms = NULL;
static idx_t nterms = 0;
static uint64_t sieved_limit = 0;

static void build(uint64_t limit) {
    unsigned char *c = (unsigned char *)malloc((size_t)(limit + 1));
    uint64_t m, k, cnt = 0, idx = 0;
    for (m = 0; m <= limit; m++) c[m] = 255;
    c[0] = 0;
    for (k = 1; k * k * k * k * k <= limit; k++) {
        uint64_t p = k * k * k * k * k;
        for (m = p; m <= limit; m++) {
            unsigned char v = c[m - p];
            if (v < 12 && (unsigned char)(v + 1) < c[m]) c[m] = (unsigned char)(v + 1);
        }
    }
    for (m = 0; m <= limit; m++) if (c[m] <= 11) cnt++;
    free(terms);
    terms = (uint64_t *)malloc((size_t)cnt * sizeof(uint64_t));
    for (m = 0; m <= limit; m++) if (c[m] <= 11) terms[idx++] = m;
    nterms = (idx_t)cnt;
    sieved_limit = limit;
    free(c);
}

seq_t a(idx_t n) {
    uint64_t limit = sieved_limit ? sieved_limit : 65536;
    if (terms == NULL || n > nterms) {
        if (terms != NULL) limit *= 2;
        build(limit);
        while (n > nterms) {
            limit *= 2;
            build(limit);
        }
    }
    return (seq_t)terms[n - 1];
}
