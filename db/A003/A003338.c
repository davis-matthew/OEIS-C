/*
 * A003338  Numbers that are the sum of 4 nonzero 4th powers.
 * https://oeis.org/A003338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sieve - mark every a^4+b^4+c^4+d^4 with 1<=a<=b<=c<=d up to a
 *            limit, then list the marked numbers in increasing order (the
 *            limit doubles until n terms exist)
 * Range:     u8 n <= 13, u16 n <= 2097, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static uint64_t *cached_terms = NULL;
static idx_t cached_len = 0;
static uint64_t cached_limit = 0;

static void build(uint64_t limit) {
    unsigned char *mark = (unsigned char *)calloc((size_t)(limit + 1), 1);
    uint64_t a, b, c, d;
    idx_t count = 0, k = 0;
    uint64_t i;
    for (a = 1; a * a * a * a <= limit; a++) {
        uint64_t s1 = a * a * a * a;
        for (b = a; s1 + b * b * b * b <= limit; b++) {
            uint64_t s2 = s1 + b * b * b * b;
            for (c = b; s2 + c * c * c * c <= limit; c++) {
                uint64_t s3 = s2 + c * c * c * c;
                for (d = c; s3 + d * d * d * d <= limit; d++) {
                    mark[s3 + d * d * d * d] = 1;
                }
            }
        }
    }
    for (i = 1; i <= limit; i++)
        if (mark[i]) count++;
    free(cached_terms);
    cached_terms = (uint64_t *)malloc((size_t)(count ? count : 1) * sizeof(uint64_t));
    for (i = 1; i <= limit; i++)
        if (mark[i]) cached_terms[k++] = i;
    cached_len = count;
    cached_limit = limit;
    free(mark);
}

seq_t a(idx_t n) {
    uint64_t limit = cached_limit ? cached_limit : ((uint64_t)1 << 16);
    if (cached_terms == NULL) build(limit);
    while (cached_len < n) {
        limit = cached_limit * 2;
        build(limit);
    }
    return (seq_t)cached_terms[n - 1];
}
