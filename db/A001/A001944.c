/*
 * A001944  Numbers that are the sum of 4 distinct squares: of form w^2 + x^2
 *          + y^2 + z^2 with 0 <= w < x < y < z.
 * https://oeis.org/A001944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sieve -- mark every w^2+x^2+y^2+z^2 with 0 <= w < x < y < z
 *            below a limit, list the marked numbers in increasing order, and
 *            double the limit until there are at least n terms
 * Range:     u8 n <= 184, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *terms = 0;
static idx_t terms_count = 0;

static void build(uint64_t L) {
    unsigned char *mark = (unsigned char *)calloc((size_t)L, 1);
    uint64_t z, y, x, w, cnt = 0, m;
    for (z = 3; z * z + 5 < L; z++) {
        for (y = 2; y < z && y * y + z * z + 1 < L; y++) {
            for (x = 1; x < y && x * x + y * y + z * z < L; x++) {
                uint64_t base = x * x + y * y + z * z;
                for (w = 0; w < x; w++) {
                    uint64_t s = base + w * w;
                    if (s >= L) break;
                    mark[s] = 1;
                }
            }
        }
    }
    for (m = 0; m < L; m++) if (mark[m]) cnt++;
    free(terms);
    terms = (uint64_t *)malloc((size_t)(cnt + 1) * sizeof(uint64_t));
    terms_count = 0;
    for (m = 0; m < L; m++) if (mark[m]) terms[terms_count++] = m;
    free(mark);
}

seq_t a(idx_t n) {
    if (n > terms_count) {
        uint64_t L = 1024;
        while (L < 2 * n + 200) L *= 2;
        build(L);
        while (n > terms_count) {
            L *= 2;
            build(L);
        }
    }
    return (seq_t)terms[n - 1];
}
