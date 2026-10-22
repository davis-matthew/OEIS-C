/*
 * A000394  Numbers of form x^2 + y^2 + 7z^2.
 * https://oeis.org/A000394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   sieve -- mark every m = x^2 + y^2 + 7*z^2 (x, y, z >= 0) up to a
 *            limit, a(n) = n-th marked m (0-based); limit doubled until
 *            enough terms
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *a394_terms = NULL;
static size_t a394_count = 0;
static uint64_t a394_limit = 0;

static void a394_build(uint64_t limit) {
    unsigned char *mark = (unsigned char *)calloc((size_t)(limit + 1), 1);
    uint64_t z, y, x, m;
    size_t cnt = 0;
    for (z = 0; 7 * z * z <= limit; z++) {
        uint64_t t = 7 * z * z;
        for (y = 0; t + y * y <= limit; y++) {
            uint64_t u = t + y * y;
            for (x = 0; u + x * x <= limit; x++)
                mark[u + x * x] = 1;
        }
    }
    for (m = 0; m <= limit; m++)
        if (mark[m]) cnt++;
    free(a394_terms);
    a394_terms = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    a394_count = 0;
    for (m = 0; m <= limit; m++)
        if (mark[m]) a394_terms[a394_count++] = m;
    free(mark);
    a394_limit = limit;
}

seq_t a(idx_t n) {
    uint64_t limit;
    if ((uint64_t)n < a394_count)
        return (seq_t)a394_terms[n];
    limit = a394_limit ? a394_limit : 64;
    if (limit < 2 * (uint64_t)n + 64) limit = 2 * (uint64_t)n + 64;
    for (;;) {
        a394_build(limit);
        if ((uint64_t)n < a394_count) break;
        limit *= 2;
    }
    return (seq_t)a394_terms[n];
}
