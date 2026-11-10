/*
 * A058894  Squarefree semiprimes A006881(m) such that |A006881(m)-A007304(m)|
 *          = 1.
 * https://oeis.org/A058894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th value A006881(m) with |A006881(m)-A007304(m)| = 1;
 *            computed by sieving distinct-prime counts and pairing the m-th
 *            semiprime with the m-th sphenic number
 * Range:     u8 none, u16 none, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A058894_MAXT 4096

static uint64_t A058894_terms[A058894_MAXT];
static uint64_t A058894_found = 0;
static uint64_t A058894_limit = 0;
static bool A058894_done = false;

static void A058894_build(uint64_t L) {
    unsigned char *c = (unsigned char *)calloc((size_t)(L + 1), 1);
    if (!c) { A058894_done = true; return; }
    for (uint64_t p = 2; p <= L; p++) {
        if (c[p] == 0) {
            for (uint64_t j = p; j <= L; j += p) c[j]++;
            if (p * p <= L) {
                for (uint64_t j = p * p; j <= L; j += p * p) c[j] |= 128;
            }
        }
    }
    uint64_t found = 0;
    uint64_t i = 5, j = 29;
    for (;;) {
        do { i++; } while (i <= L && c[i] != 2);
        do { j++; } while (j <= L && c[j] != 3);
        if (i > L || j > L) break;
        if ((i > j ? i - j : j - i) == 1) {
            if (found < A058894_MAXT) A058894_terms[found++] = i;
        }
    }
    A058894_found = found;
    A058894_limit = L;
    free(c);
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n > A058894_MAXT) return (seq_t)0;
    while (A058894_found < n) {
        if (A058894_done) return (seq_t)0;
        uint64_t L = A058894_limit ? A058894_limit * 4 : 2000000;
        if (L > ((uint64_t)1 << 28)) { A058894_done = true; return (seq_t)0; }
        A058894_build(L);
    }
    return (seq_t)A058894_terms[n - 1];
}
