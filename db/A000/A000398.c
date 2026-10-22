/*
 * A000398  Numbers of form x^2 + 2y^2 + 2yz + 4z^2.
 * https://oeis.org/A000398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sieve -- mark every m = x^2 + 2y^2 + 2yz + 4z^2 <= L over all
 *            integers x, y, z (y and z may be negative), then list the marked
 *            m in increasing order; L doubles as needed
 * Range:     u8 n <= 238, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a398_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a398_isqrt.h"

static uint64_t *a398_terms = 0;
static idx_t a398_count = 0;
static uint64_t a398_limit = 0;

static void a398_build(uint64_t L) {
    unsigned char *Q = (unsigned char *)calloc((size_t)L + 1, 1);
    unsigned char *M = (unsigned char *)calloc((size_t)L + 1, 1);
    long long B = a398_isqrt((long long)L) + 1;
    long long y, z, x;
    uint64_t q, m;
    idx_t c = 0;
    for (z = -B; z <= B; z++) {
        for (y = -B; y <= B; y++) {
            long long v = 2 * y * y + 2 * y * z + 4 * z * z;
            if (v >= 0 && (uint64_t)v <= L) Q[v] = 1;
        }
    }
    for (q = 0; q <= L; q++) {
        if (!Q[q]) continue;
        for (x = 0; q + (uint64_t)(x * x) <= L; x++) M[q + (uint64_t)(x * x)] = 1;
    }
    for (m = 0; m <= L; m++) if (M[m]) c++;
    free(a398_terms);
    a398_terms = (uint64_t *)malloc((size_t)(c + 1) * sizeof(uint64_t));
    c = 0;
    for (m = 0; m <= L; m++) if (M[m]) a398_terms[c++] = m;
    a398_count = c;
    a398_limit = L;
    free(Q);
    free(M);
}

seq_t a(idx_t n) {
    if (a398_count < n) {
        uint64_t L = a398_limit ? a398_limit : 2 * (uint64_t)n + 100;
        if (L < 2 * (uint64_t)n + 100) L = 2 * (uint64_t)n + 100;
        a398_build(L);
        while (a398_count < n) a398_build(a398_limit * 2);
    }
    return (seq_t)a398_terms[n - 1];
}
