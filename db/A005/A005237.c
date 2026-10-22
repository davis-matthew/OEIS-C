/*
 * A005237  Numbers k such that k and k+1 have the same number of divisors.
 * https://oeis.org/A005237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) is the n-th k with d(k) = d(k+1), where d(k) counts
 *            divisors. Uses a divisor-count sieve that doubles its range as
 *            needed
 * Range:     u8 n <= 42, u16 n <= 6976, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static uint64_t *a005237_terms = NULL;
static size_t a005237_nterms = 0;
static size_t a005237_cap = 0;
static uint64_t a005237_scanned = 0; /* all k <= scanned have been checked */
static uint64_t a005237_limit = 0;

static void a005237_push(uint64_t k) {
    if (a005237_nterms == a005237_cap) {
        size_t nc = a005237_cap ? a005237_cap * 2 : 1024;
        a005237_terms = (uint64_t *)realloc(a005237_terms, nc * sizeof(uint64_t));
        a005237_cap = nc;
    }
    a005237_terms[a005237_nterms++] = k;
}

static void a005237_extend(void) {
    uint64_t L = a005237_limit ? a005237_limit * 2 : 4096;
    uint32_t *d = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    uint64_t i, j, k;
    for (i = 1; i <= L; i++)
        for (j = i; j <= L; j += i)
            d[j]++;
    for (k = a005237_scanned + 1; k + 1 <= L; k++)
        if (d[k] == d[k + 1])
            a005237_push(k);
    a005237_scanned = L - 1;
    a005237_limit = L;
    free(d);
}

seq_t a(idx_t n) {
    while ((idx_t)a005237_nterms < n)
        a005237_extend();
    return (seq_t)a005237_terms[n - 1];
}
