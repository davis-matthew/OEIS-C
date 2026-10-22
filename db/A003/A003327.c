/*
 * A003327  Numbers that are the sum of 4 positive cubes in 1 or more way.
 * https://oeis.org/A003327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sieve -- mark every m < L with m = a^3+b^3+c^3+d^3, 1 <= a <= b
 *            <= c <= d; a(n) is the n-th marked number (L doubles until there
 *            are n terms)
 * Range:     u8 n <= 56, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *a3327_terms = NULL;
static idx_t a3327_count = 0;
static uint64_t a3327_limit = 0;

static void a3327_build(uint64_t L) {
    unsigned char *mark = (unsigned char *)calloc((size_t)L, 1);
    uint64_t a, b, c, d, cnt = 0, m;
    if (!mark) return;
    for (a = 1; 4 * a * a * a < L; a++) {
        uint64_t s1 = a * a * a;
        for (b = a; s1 + 3 * b * b * b < L; b++) {
            uint64_t s2 = s1 + b * b * b;
            for (c = b; s2 + 2 * c * c * c < L; c++) {
                uint64_t s3 = s2 + c * c * c;
                for (d = c; s3 + d * d * d < L; d++)
                    mark[s3 + d * d * d] = 1;
            }
        }
    }
    for (m = 0; m < L; m++) if (mark[m]) cnt++;
    free(a3327_terms);
    a3327_terms = (uint64_t *)malloc((size_t)(cnt ? cnt : 1) * sizeof(uint64_t));
    a3327_count = 0;
    if (a3327_terms) {
        for (m = 0; m < L; m++) if (mark[m]) a3327_terms[a3327_count++] = m;
    }
    a3327_limit = L;
    free(mark);
}

seq_t a(idx_t n) {
    uint64_t L = a3327_limit ? a3327_limit : 1024;
    if (n == 0) return (seq_t)0;
    while (a3327_count < n) {
        if (a3327_limit) L *= 2;
        a3327_build(L);
        if (!a3327_terms) return (seq_t)0;
    }
    return (seq_t)a3327_terms[n - 1];
}
