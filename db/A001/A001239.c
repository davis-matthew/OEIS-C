/*
 * A001239  Numbers that are the sum of 3 nonnegative cubes in more than 1
 *          way.
 * https://oeis.org/A001239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve -- count representations m = x^3 + y^3 + z^3 with 0 <= x
 *            <= y <= z up to a limit that doubles as needed; a(n) is the n-th
 *            m with at least 2 representations
 * Range:     u8 n <= 2, u16 n <= 868, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *a1239_terms = NULL;
static size_t a1239_nterms = 0;
static uint64_t a1239_lim = 0;

static void a1239_build(uint64_t lim) {
    unsigned char *cnt = (unsigned char *)calloc((size_t)lim + 1, 1);
    uint64_t x, y, z;
    size_t k = 0, m;
    for (x = 0; x * x * x <= lim; x++) {
        uint64_t x3 = x * x * x;
        for (y = x; x3 + y * y * y <= lim; y++) {
            uint64_t s2 = x3 + y * y * y;
            for (z = y; ; z++) {
                uint64_t s = s2 + z * z * z;
                if (s > lim) break;
                if (cnt[s] < 2) cnt[s]++;
            }
        }
    }
    for (m = 0; m <= (size_t)lim; m++)
        if (cnt[m] >= 2) k++;
    free(a1239_terms);
    a1239_terms = (uint64_t *)malloc((k ? k : 1) * sizeof(uint64_t));
    k = 0;
    for (m = 0; m <= (size_t)lim; m++)
        if (cnt[m] >= 2) a1239_terms[k++] = (uint64_t)m;
    a1239_nterms = k;
    free(cnt);
}

seq_t a(idx_t n) {
    while (a1239_terms == NULL || (idx_t)a1239_nterms < n) {
        a1239_lim = a1239_lim ? a1239_lim * 2 : 4096;
        a1239_build(a1239_lim);
    }
    return (seq_t)a1239_terms[n - 1];
}
