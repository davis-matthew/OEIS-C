/*
 * A002366  Numbers x such that x^2 + y^2 = p^2 = A002144(n)^2, x < y.
 * https://oeis.org/A002366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min(2*a*b, b^2 - a^2) where p = A002144(n) = a^2 + b^2
 *            with a < b (primes p == 1 mod 4 are found with a growing sieve)
 * Range:     u8 n <= 41, u16 n <= 4565, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a2366_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a2366_isqrt.h"

static uint64_t *a2366_pr = NULL;
static size_t a2366_cnt = 0;
static uint64_t a2366_lim = 0;

static void a2366_build(uint64_t lim)
{
    unsigned char *comp = (unsigned char *)malloc((size_t)(lim + 1));
    uint64_t i, j;
    size_t cap = 1024;
    memset(comp, 0, (size_t)(lim + 1));
    free(a2366_pr);
    a2366_pr = (uint64_t *)malloc(cap * sizeof(uint64_t));
    a2366_cnt = 0;
    for (i = 2; i <= lim; i++) {
        if (comp[i]) continue;
        if (i % 4 == 1) {
            if (a2366_cnt == cap) {
                cap *= 2;
                a2366_pr = (uint64_t *)realloc(a2366_pr, cap * sizeof(uint64_t));
            }
            a2366_pr[a2366_cnt++] = i;
        }
        if (i <= lim / i)
            for (j = i * i; j <= lim; j += i) comp[j] = 1;
    }
    free(comp);
    a2366_lim = lim;
}

seq_t a(idx_t n)
{
    uint64_t p, s, t, x, y, av = 0, bv = 0;
    if (a2366_lim == 0) a2366_build(1 << 16);
    while ((uint64_t)a2366_cnt < n) a2366_build(a2366_lim * 2);
    p = a2366_pr[n - 1];
    for (s = 1; 2 * s * s < p; s++) {
        uint64_t r = p - s * s;
        t = a2366_isqrt(r);
        if (t * t == r) {
            av = s;
            bv = t;
            break;
        }
    }
    x = 2 * av * bv;
    y = bv * bv - av * av;
    return (seq_t)(x < y ? x : y);
}
