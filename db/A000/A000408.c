/*
 * A000408  Numbers that are the sum of three nonzero squares.
 * https://oeis.org/A000408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve -- mark every x^2+y^2+z^2 <= M with 1 <= x <= y <= z, list
 *            the marked numbers in increasing order, and return the n-th one;
 *            M is doubled until at least n terms are found
 * Range:     u8 n <= 188, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *a408_terms = 0;
static uint64_t a408_count = 0;

static void a408_build(uint64_t M) {
    unsigned char *mark = (unsigned char *)calloc((size_t)M + 1, 1);
    uint64_t x, y, z, cnt = 0, k;
    for (x = 1; 3 * x * x <= M; x++) {
        for (y = x; x * x + 2 * y * y <= M; y++) {
            uint64_t s = x * x + y * y;
            for (z = y; s + z * z <= M; z++)
                mark[s + z * z] = 1;
        }
    }
    for (k = 1; k <= M; k++) if (mark[k]) cnt++;
    free(a408_terms);
    a408_terms = (uint64_t *)malloc((size_t)(cnt ? cnt : 1) * sizeof(uint64_t));
    cnt = 0;
    for (k = 1; k <= M; k++) if (mark[k]) a408_terms[cnt++] = k;
    a408_count = cnt;
    free(mark);
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (a408_count < n) {
        uint64_t M = 2 * (uint64_t)n + 64;
        a408_build(M);
        while (a408_count < n) {
            M *= 2;
            a408_build(M);
        }
    }
    return (seq_t)a408_terms[n - 1];
}
