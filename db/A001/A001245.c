/*
 * A001245  Numbers that are the sum of 4 cubes in more than 1 way.
 * https://oeis.org/A001245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve -- count representations x = a^3+b^3+c^3+d^3 with 0 <= a
 *            <= b <= c <= d up to a growing limit; terms are the x with at
 *            least 2 representations
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *s_terms = NULL;
static size_t s_nterms = 0;
static uint64_t s_lim = 0;

static void a001245_build(uint64_t L) {
    unsigned char *cnt = (unsigned char *)calloc((size_t)(L + 1), 1);
    uint64_t a, b, c, d;
    size_t k = 0, x;
    for (a = 0; a * a * a <= L; a++) {
        uint64_t s1 = a * a * a;
        for (b = a; s1 + b * b * b <= L; b++) {
            uint64_t s2 = s1 + b * b * b;
            for (c = b; s2 + c * c * c <= L; c++) {
                uint64_t s3 = s2 + c * c * c;
                for (d = c; s3 + d * d * d <= L; d++) {
                    uint64_t s = s3 + d * d * d;
                    if (cnt[s] < 2) cnt[s]++;
                }
            }
        }
    }
    for (x = 0; x <= (size_t)L; x++)
        if (cnt[x] >= 2) k++;
    free(s_terms);
    s_terms = (uint64_t *)malloc((k + 1) * sizeof(uint64_t));
    k = 0;
    for (x = 0; x <= (size_t)L; x++)
        if (cnt[x] >= 2) s_terms[k++] = (uint64_t)x;
    free(cnt);
    s_nterms = k;
    s_lim = L;
}

seq_t a(idx_t n) {
    while (s_nterms < n) {
        a001245_build(s_lim ? s_lim * 2 : 4096);
    }
    return (seq_t)s_terms[n - 1];
}
