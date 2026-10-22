/*
 * A001974  Numbers that are the sum of 3 distinct squares, i.e., numbers of
 *          the form x^2 + y^2 + z^2 with 0 <= x < y < z.
 * https://oeis.org/A001974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n-th number of the form x^2 + y^2 + z^2 with 0 <= x < y <
 *            z; computed by marking all such sums up to a bound (a sieve),
 *            enlarging the bound until there are n terms
 * Range:     u8 n <= 169, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *a001974_terms = NULL;
static uint64_t a001974_count = 0;

static void a001974_build(uint64_t limit) {
    unsigned char *mark = (unsigned char *)malloc((size_t)(limit + 1));
    uint64_t x, y, z, cnt = 0, m;
    memset(mark, 0, (size_t)(limit + 1));
    for (x = 0; x * x <= limit; x++) {
        for (y = x + 1; x * x + y * y <= limit; y++) {
            for (z = y + 1; x * x + y * y + z * z <= limit; z++) {
                mark[x * x + y * y + z * z] = 1;
            }
        }
    }
    for (m = 1; m <= limit; m++) if (mark[m]) cnt++;
    free(a001974_terms);
    a001974_terms = (uint64_t *)malloc((size_t)(cnt + 1) * sizeof(uint64_t));
    a001974_count = 0;
    for (m = 1; m <= limit; m++) if (mark[m]) a001974_terms[a001974_count++] = m;
    free(mark);
}

seq_t a(idx_t n) {
    if (n > a001974_count) {
        uint64_t limit = 2 * (uint64_t)n + 64;
        for (;;) {
            a001974_build(limit);
            if (a001974_count >= n) break;
            limit *= 2;
        }
    }
    return (seq_t)a001974_terms[n - 1];
}
