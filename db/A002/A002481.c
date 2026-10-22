/*
 * A002481  Numbers of form x^2 + 6y^2.
 * https://oeis.org/A002481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve; mark all x^2 + 6*y^2 (x, y >= 0) up to a limit that
 *            doubles until it holds n terms, a(n) = n-th marked number in
 *            increasing order (a(1) = 0)
 * Range:     u8 n <= 76, u16 n <= 2064, u32 n <= 2064, u64 n <= 2064,
 *            u128 n <= 2064, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *a002481_terms = NULL;
static uint64_t a002481_cnt = 0;

static void a002481_build(uint64_t L) {
    unsigned char *mark = (unsigned char *)malloc((size_t)(L + 1));
    uint64_t x, y, m, c = 0;
    memset(mark, 0, (size_t)(L + 1));
    for (y = 0; 6 * y * y <= L; y++) {
        uint64_t b = 6 * y * y;
        for (x = 0; x * x + b <= L; x++) mark[x * x + b] = 1;
    }
    for (m = 0; m <= L; m++) if (mark[m]) c++;
    free(a002481_terms);
    a002481_terms = (uint64_t *)malloc((size_t)(c ? c : 1) * sizeof(uint64_t));
    c = 0;
    for (m = 0; m <= L; m++) if (mark[m]) a002481_terms[c++] = m;
    a002481_cnt = c;
    free(mark);
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (a002481_cnt < n) {
        uint64_t L = 8 * (uint64_t)n + 64;
        for (;;) {
            a002481_build(L);
            if (a002481_cnt >= n) break;
            L *= 2;
        }
    }
    return (seq_t)a002481_terms[n - 1];
}
