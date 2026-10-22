/*
 * A006073  Numbers k such that k, k+1 and k+2 all have the same number of
 *          distinct prime divisors.
 * https://oeis.org/A006073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 5271, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sieve: omega(m) (number of distinct primes dividing m) for all m up to a limit, then list the k with omega(k) = omega(k+1) = omega(k+2); the limit doubles until there are n terms */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *A006073_terms = 0;
static uint64_t A006073_count = 0;
static uint64_t A006073_lim = 0;

static void A006073_build(uint64_t L) {
    uint64_t N = L + 3, p, m, k, cap;
    unsigned char *om = (unsigned char *)malloc((size_t)N);
    memset(om, 0, (size_t)N);
    for (p = 2; p < N; p++) {
        if (om[p] == 0) {
            for (m = p; m < N; m += p) om[m]++;
        }
    }
    cap = 1024;
    free(A006073_terms);
    A006073_terms = (uint64_t *)malloc((size_t)cap * sizeof(uint64_t));
    A006073_count = 0;
    for (k = 1; k <= L; k++) {
        if (om[k] == om[k + 1] && om[k + 1] == om[k + 2]) {
            if (A006073_count == cap) {
                cap *= 2;
                A006073_terms = (uint64_t *)realloc(A006073_terms, (size_t)cap * sizeof(uint64_t));
            }
            A006073_terms[A006073_count++] = k;
        }
    }
    free(om);
    A006073_lim = L;
}

seq_t a(idx_t n) {
    uint64_t L = A006073_lim ? A006073_lim : 65536;
    while (A006073_count < n) {
        if (A006073_lim >= L) L *= 2;
        A006073_build(L);
    }
    return (seq_t)A006073_terms[n - 1];
}
