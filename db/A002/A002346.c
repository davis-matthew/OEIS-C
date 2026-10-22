/*
 * A002346  Consider all primes of form p = (x^2 + 11y^2 )/4; sequence gives
 *          values of x.
 * https://oeis.org/A002346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- for each prime p in increasing order (p = 11 or p a
 *            square mod 11), find y >= 0 with 4p - 11*y^2 = x^2 a perfect
 *            square; a(n) = x for the n-th such prime
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   a002346_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a002346_isqrt.h"

static uint64_t *a002346_xs = NULL;
static size_t a002346_cnt = 0;

static void a002346_build(uint64_t limit) {
    unsigned char *comp = (unsigned char *)calloc((size_t)(limit + 1), 1);
    uint64_t i, j, p;
    size_t cap = 1024, cnt = 0;
    uint64_t *xs = (uint64_t *)malloc(cap * sizeof(uint64_t));
    for (i = 2; i * i <= limit; i++)
        if (!comp[i])
            for (j = i * i; j <= limit; j += i) comp[j] = 1;
    for (p = 2; p <= limit; p++) {
        uint64_t r, fp, y;
        if (comp[p]) continue;
        r = p % 11;
        if (!(p == 11 || r == 1 || r == 3 || r == 4 || r == 5 || r == 9)) continue;
        fp = 4 * p;
        for (y = 0; 11 * y * y <= fp; y++) {
            uint64_t xsq = fp - 11 * y * y;
            uint64_t x = a002346_isqrt(xsq);
            if (x * x == xsq) {
                if (cnt == cap) {
                    cap *= 2;
                    xs = (uint64_t *)realloc(xs, cap * sizeof(uint64_t));
                }
                xs[cnt++] = x;
                break;
            }
        }
    }
    free(comp);
    free(a002346_xs);
    a002346_xs = xs;
    a002346_cnt = cnt;
}

seq_t a(idx_t n) {
    static uint64_t limit = 0;
    if (n < 1) return (seq_t)0;
    while ((idx_t)a002346_cnt < n) {
        limit = limit ? limit * 2 : 4096;
        a002346_build(limit);
    }
    return (seq_t)a002346_xs[n - 1];
}
