/*
 * A002368  Let p = A007645(n) be the n-th generalized cuban prime and write
 *          p^2 = x^2 + 3*y^2 with y > 0; a(n) = y.
 * https://oeis.org/A002368
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   p = A007645(n) is the (n-1)-th prime == 1 (mod 3) for n >= 2;
 *            write p = u^2 + 3*v^2, then p^2 = (u^2 - 3*v^2)^2 + 3*(2*u*v)^2,
 *            so a(n) = 2*u*v. Primes are found by a sieve
 * Range:     u8 n <= 45, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   c7645_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/c7645_isqrt.h"

static uint64_t *c7645_pr = NULL;
static size_t c7645_cnt = 0;

static void c7645_build(uint64_t lim) {
    unsigned char *comp = (unsigned char *)malloc((size_t)(lim + 1));
    uint64_t i, j;
    size_t cap = 1024, cnt = 0;
    uint64_t *pr = (uint64_t *)malloc(cap * sizeof(uint64_t));
    memset(comp, 0, (size_t)(lim + 1));
    for (i = 2; i <= lim; i++) {
        if (comp[i]) continue;
        if (i * i <= lim)
            for (j = i * i; j <= lim; j += i) comp[j] = 1;
        if (i % 3 == 1) {
            if (cnt == cap) {
                cap *= 2;
                pr = (uint64_t *)realloc(pr, cap * sizeof(uint64_t));
            }
            pr[cnt++] = i;
        }
    }
    free(comp);
    free(c7645_pr);
    c7645_pr = pr;
    c7645_cnt = cnt;
}

seq_t a(idx_t n) {
    uint64_t k, p, v, lim;
    if (n < 2) return (seq_t)0;
    k = (uint64_t)(n - 2); /* 0-based index among primes == 1 mod 3 */
    lim = 1024;
    while (c7645_cnt <= k) {
        while (lim < 30 * (k + 10)) lim *= 2;
        c7645_build(lim);
        lim *= 2;
    }
    p = c7645_pr[k];
    for (v = 1; 3 * v * v < p; v++) {
        uint64_t rem = p - 3 * v * v;
        uint64_t u = c7645_isqrt(rem);
        if (u * u == rem && u > 0) {
            seq_t r = (seq_t)2;
            r *= (seq_t)u;
            r *= (seq_t)v;
            return r;
        }
    }
    return (seq_t)0;
}
