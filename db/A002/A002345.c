/*
 * A002345  Numbers y such that p = x^2 + 7y^2, with prime p = A033207(n).
 * https://oeis.org/A002345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve the primes p and keep those of the form x^2 + 7*y^2
 *            (A033207); a(n) is the y for the n-th such prime, found by
 *            searching y = 1, 2, ... until p - 7*y^2 is a perfect square
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isqrt_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/isqrt_u.h"

static uint32_t *ys = NULL;
static idx_t ys_len = 0;

static void build(uint64_t limit) {
    unsigned char *comp = (unsigned char *)calloc(limit + 1, 1);
    uint32_t *buf = (uint32_t *)malloc(sizeof(uint32_t) * (limit / 2 + 16));
    idx_t cnt = 0;
    uint64_t i, j;
    for (i = 2; i * i <= limit; i++)
        if (!comp[i])
            for (j = i * i; j <= limit; j += i) comp[j] = 1;
    for (i = 2; i <= limit; i++) {
        uint64_t y;
        if (comp[i]) continue;
        for (y = 1; 7 * y * y <= i; y++) {
            uint64_t rem = i - 7 * y * y;
            uint64_t x = isqrt_u(rem);
            if (x * x == rem) {
                buf[cnt++] = (uint32_t)y;
                break;
            }
        }
    }
    free(comp);
    if (ys) free(ys);
    ys = buf;
    ys_len = cnt;
}

seq_t a(idx_t n) {
    uint64_t limit = 1024;
    while (ys_len < n) {
        build(limit);
        limit *= 2;
    }
    return (seq_t)ys[n - 1];
}
