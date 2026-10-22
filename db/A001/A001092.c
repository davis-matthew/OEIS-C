/*
 * A001092  Union of all numbers {p, q} where p and q are both primes or
 *          powers of primes and q = p+2.
 * https://oeis.org/A001092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sieve search: list m >= 1 such that m is 1, a prime or a prime
 *            power, and m-2 or m+2 is also one
 * Range:     u8 n <= 50, u16 n <= 1795, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *a1092_terms = NULL;
static idx_t a1092_count = 0;

static void a1092_build(uint64_t L) {
    uint64_t M = L + 3, p, q, m;
    unsigned char *comp = (unsigned char *)calloc((size_t)M, 1);
    unsigned char *pp = (unsigned char *)calloc((size_t)M, 1);
    idx_t cnt = 0;
    pp[1] = 1;
    for (p = 2; p < M; p++) {
        if (comp[p]) continue;
        for (q = p * p; q < M; q += p) comp[q] = 1;
        for (q = p; q < M; q *= p) {
            pp[q] = 1;
            if (q > (M - 1) / p) break;
        }
    }
    free(a1092_terms);
    a1092_terms = (uint64_t *)malloc((size_t)(L + 1) * sizeof(uint64_t));
    for (m = 1; m <= L; m++) {
        if (pp[m] && (pp[m + 2] || (m >= 3 && pp[m - 2])))
            a1092_terms[cnt++] = m;
    }
    a1092_count = cnt;
    free(comp);
    free(pp);
}

seq_t a(idx_t n) {
    uint64_t L = 1 << 16;
    if (n < 1) return 0;
    while (a1092_count < n) {
        a1092_build(L);
        L *= 2;
    }
    return (seq_t)a1092_terms[n - 1];
}
