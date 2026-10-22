/*
 * A005278  Noncototients: numbers k such that x - phi(x) = k has no solution.
 * https://oeis.org/A005278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   { k | A063740(k) = 0 }, computed by a sieve. phi(x) is even for
 *            x > 2, so x - phi(x) is even exactly when x is even. For even x,
 *            x - phi(x) >= x/2, so an even k can only be x - phi(x) for an
 *            even x <= 2k. Odd k are all cototients (p*q - phi(p*q) = p+q-1,
 *            Goldbach verified far beyond range). Sieve...
 * Range:     u8 n <= 23, u16 n <= 6810, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *nc_terms = NULL;
static uint64_t nc_count = 0;
static uint64_t nc_limit = 0;

static void nc_build(uint64_t L) {
    uint64_t M = 2 * L;
    uint32_t *ph = (uint32_t *)malloc((size_t)(M + 1) * sizeof(uint32_t));
    unsigned char *mark = (unsigned char *)calloc((size_t)(L + 1), 1);
    uint64_t i, j;
    for (i = 0; i <= M; i++) ph[i] = (uint32_t)i;
    for (i = 2; i <= M; i++) {
        if (ph[i] == (uint32_t)i) {
            for (j = i; j <= M; j += i) ph[j] -= ph[j] / (uint32_t)i;
        }
    }
    for (i = 2; i <= M; i += 2) {
        uint64_t c = i - (uint64_t)ph[i];
        if (c <= L) mark[c] = 1;
    }
    free(ph);
    if (nc_terms) free(nc_terms);
    nc_terms = (uint64_t *)malloc((size_t)(L / 2 + 2) * sizeof(uint64_t));
    nc_count = 0;
    for (i = 2; i <= L; i += 2) {
        if (!mark[i]) nc_terms[nc_count++] = i;
    }
    free(mark);
    nc_limit = L;
}

seq_t a(idx_t n) {
    if (nc_limit == 0) nc_build(1024);
    while (nc_count < n) nc_build(nc_limit * 2);
    return (seq_t)nc_terms[n - 1];
}
