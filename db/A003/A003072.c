/*
 * A003072  Numbers that are the sum of 3 positive cubes.
 * https://oeis.org/A003072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   sieve -- mark every a^3+b^3+c^3 with 1 <= a <= b <= c up to a
 *            limit (doubled as needed), then a(n) is the n-th marked number
 * Range:     u8 n <= 35, u16 n <= 7151, u32 n <= 9363, u64 n <= 9363,
 *            u128 n <= 9363, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static uint64_t *A003072_terms = NULL;
static size_t A003072_cnt = 0;
static uint64_t A003072_lim = 0;

static void A003072_build(uint64_t lim) {
    unsigned char *mark = (unsigned char *)calloc((size_t)lim + 1, 1);
    uint64_t x, y, z, cnt = 0, i;
    for (x = 1; 3 * x * x * x <= lim; x++) {
        uint64_t x3 = x * x * x;
        for (y = x; x3 + 2 * y * y * y <= lim; y++) {
            uint64_t s2 = x3 + y * y * y;
            for (z = y; ; z++) {
                uint64_t s = s2 + z * z * z;
                if (s > lim) break;
                mark[s] = 1;
            }
        }
    }
    for (i = 0; i <= lim; i++) if (mark[i]) cnt++;
    free(A003072_terms);
    A003072_terms = (uint64_t *)malloc((size_t)(cnt ? cnt : 1) * sizeof(uint64_t));
    A003072_cnt = 0;
    for (i = 0; i <= lim; i++) if (mark[i]) A003072_terms[A003072_cnt++] = i;
    free(mark);
    A003072_lim = lim;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    while (A003072_cnt < n) {
        uint64_t lim = A003072_lim ? A003072_lim * 2 : 4096;
        A003072_build(lim);
    }
    return (seq_t)A003072_terms[n - 1];
}
