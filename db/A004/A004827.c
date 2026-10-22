/*
 * A004827  Numbers that are the sum of at most 5 positive cubes.
 * https://oeis.org/A004827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve -- a number m is a term iff the minimal number of positive
 *            cubes summing to m is <= 5 (computed by DP: c(0)=0, c(m) = 1 +
 *            min_{k^3<=m} c(m-k^3)); a(n) is the n-th such m, starting from
 *            a(1) = 0
 * Range:     u8 n <= 166, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static unsigned long long *A004827_terms = NULL;
static size_t A004827_nterms = 0;
static size_t A004827_limit = 0;

static void A004827_sieve(size_t L) {
    unsigned char *cnt = (unsigned char *)malloc(L + 1);
    size_t m, cap = 0, k;
    cnt[0] = 0;
    for (m = 1; m <= L; m++) {
        unsigned char best = 6;
        for (k = 1; k * k * k <= m; k++) {
            unsigned char v = (unsigned char)(cnt[m - k * k * k] + 1);
            if (v < best) best = v;
        }
        cnt[m] = best;
    }
    free(A004827_terms);
    A004827_terms = NULL;
    A004827_nterms = 0;
    for (m = 0; m <= L; m++) {
        if (cnt[m] <= 5) {
            if (A004827_nterms == cap) {
                cap = cap ? cap * 2 : 1024;
                A004827_terms = (unsigned long long *)realloc(A004827_terms, cap * sizeof(unsigned long long));
            }
            A004827_terms[A004827_nterms++] = (unsigned long long)m;
        }
    }
    free(cnt);
    A004827_limit = L;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    while ((idx_t)A004827_nterms < n) {
        size_t L = A004827_limit ? A004827_limit * 2 : 0;
        if (L < (size_t)n * 2) L = (size_t)n * 2;
        if (L < 100) L = 100;
        A004827_sieve(L);
    }
    return (seq_t)A004827_terms[n - 1];
}
