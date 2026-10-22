/*
 * A002333  Numbers y such that p = x^2 + 2y^2, with prime p = A033203(n).
 * https://oeis.org/A002333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   let p = A033203(n), the n-th prime with p = 2 or p == 1, 3 (mod
 *            8). a(n) is the unique y >= 1 with p - 2*y^2 a perfect square.
 *            Found by sieving primes and then searching over y
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a2333_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a2333_isqrt.h"

static uint64_t *a2333_ps = 0;
static size_t a2333_cnt = 0;
static uint64_t a2333_lim = 0;

static void a2333_build(uint64_t lim) {
    unsigned char *c = (unsigned char *)malloc((size_t)lim + 1);
    uint64_t i, j;
    size_t cap = 1024;
    memset(c, 0, (size_t)lim + 1);
    free(a2333_ps);
    a2333_ps = (uint64_t *)malloc(cap * sizeof(uint64_t));
    a2333_cnt = 0;
    for (i = 2; i <= lim; i++) {
        if (c[i]) continue;
        if (i * i <= lim)
            for (j = i * i; j <= lim; j += i) c[j] = 1;
        if (i == 2 || i % 8 == 1 || i % 8 == 3) {
            if (a2333_cnt == cap) {
                cap *= 2;
                a2333_ps = (uint64_t *)realloc(a2333_ps, cap * sizeof(uint64_t));
            }
            a2333_ps[a2333_cnt++] = i;
        }
    }
    free(c);
    a2333_lim = lim;
}

seq_t a(idx_t n) {
    uint64_t p, y, r, lim;
    if (n < 1) return 0;
    lim = a2333_lim ? a2333_lim : 1024;
    while (a2333_cnt < (size_t)n) {
        if (a2333_lim) lim *= 2;
        a2333_build(lim);
    }
    p = a2333_ps[n - 1];
    for (y = 1; 2 * y * y <= p; y++) {
        uint64_t rest = p - 2 * y * y;
        r = a2333_isqrt(rest);
        if (r * r == rest) return (seq_t)y;
    }
    return 0;
}
