/*
 * A006285  Odd numbers not of form p + 2^k (de Polignac numbers).
 * https://oeis.org/A006285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   sieve search -- a(n) is the n-th odd m such that m - 2^k is not
 *            prime for every 2^k < m
 * Range:     u8 n <= 4, u16 n <= 2195, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *dp_terms = NULL;
static size_t dp_count = 0;
static uint64_t dp_limit = 0;

static void dp_build(uint64_t L) {
    unsigned char *comp = (unsigned char *)malloc((size_t)(L + 1));
    uint64_t i, j, m;
    size_t cap = 1024;
    memset(comp, 0, (size_t)(L + 1));
    comp[0] = 1;
    if (L >= 1) comp[1] = 1;
    for (i = 2; i * i <= L; i++)
        if (!comp[i])
            for (j = i * i; j <= L; j += i) comp[j] = 1;
    free(dp_terms);
    dp_terms = (uint64_t *)malloc(cap * sizeof(uint64_t));
    dp_count = 0;
    for (m = 1; m <= L; m += 2) {
        uint64_t p = 1;
        int ok = 1;
        while (p < m) {
            if (!comp[m - p]) { ok = 0; break; }
            p <<= 1;
        }
        if (ok) {
            if (dp_count == cap) {
                cap *= 2;
                dp_terms = (uint64_t *)realloc(dp_terms, cap * sizeof(uint64_t));
            }
            dp_terms[dp_count++] = m;
        }
    }
    free(comp);
    dp_limit = L;
}

seq_t a(idx_t n) {
    while ((uint64_t)dp_count < (uint64_t)n) {
        uint64_t L = (uint64_t)n * 24 + 1000;
        if (L <= dp_limit) L = dp_limit * 2;
        dp_build(L);
    }
    return (seq_t)dp_terms[n - 1];
}
