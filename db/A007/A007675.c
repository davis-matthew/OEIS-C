/*
 * A007675  Numbers m such that m, m+1 and m+2 are squarefree.
 * https://oeis.org/A007675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   sieve for squarefree numbers; a(n) = n-th m with m, m+1, m+2 all
 *            squarefree
 * Range:     u8 n <= 35, u16 n <= 8227, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static uint64_t *A007675_terms = NULL;
static uint64_t A007675_count = 0;
static uint64_t A007675_limit = 0;

static void A007675_build(uint64_t L) {
    uint64_t sz = L + 3;
    unsigned char *sf = (unsigned char *)malloc((size_t)sz);
    uint64_t p, j, m, cnt = 0;
    memset(sf, 1, (size_t)sz);
    sf[0] = 0;
    for (p = 2; p * p < sz; p++) {
        uint64_t q = p * p;
        for (j = q; j < sz; j += q) sf[j] = 0;
    }
    for (m = 1; m <= L; m++)
        if (sf[m] && sf[m + 1] && sf[m + 2]) cnt++;
    free(A007675_terms);
    A007675_terms = (uint64_t *)malloc((size_t)(cnt + 1) * sizeof(uint64_t));
    cnt = 0;
    for (m = 1; m <= L; m++)
        if (sf[m] && sf[m + 1] && sf[m + 2]) A007675_terms[cnt++] = m;
    A007675_count = cnt;
    A007675_limit = L;
    free(sf);
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (A007675_count < n) {
        uint64_t L = A007675_limit ? A007675_limit : 1024;
        while (L < 9 * (uint64_t)n + 100) L *= 2;
        A007675_build(L);
        while (A007675_count < n) {
            L *= 2;
            A007675_build(L);
        }
    }
    return (seq_t)A007675_terms[n - 1];
}
