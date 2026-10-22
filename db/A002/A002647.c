/*
 * A002647  Sextan primes: p = (x^6 + y^6)/(x^2 + y^2).
 * https://oeis.org/A002647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. a(n) = n-th smallest prime of the form x^4 - x^2*y^2 +
 *            y^4 = (x^6+y^6)/(x^2+y^2), x > y >= 1; enumerate (x,y) up to a
 *            growing bound, test with Miller-Rabin, sort
 * Range:     u8 n <= 5, u16 n <= 46, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   s2647_cmp, s2647_isprime, s2647_mulmod, s2647_powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/s2647_cmp.h"
#include "../utils/s2647_isprime.h"
#include "../utils/s2647_mulmod.h"
#include "../utils/s2647_powmod.h"

static uint64_t *s2647_list = NULL;
static size_t s2647_count = 0;
static uint64_t s2647_limit = 0;

static void s2647_build(uint64_t L) {
    size_t cap = 1024, cnt = 0;
    uint64_t *buf = (uint64_t *)malloc(cap * sizeof(uint64_t));
    uint64_t x, y;
    for (x = 2; ; x++) {
        uint64_t x2 = x * x;
        uint64_t x4 = x2 * x2;
        if (x4 / 4 * 3 > L) break;
        for (y = 1; y < x; y++) {
            uint64_t y2 = y * y;
            uint64_t f = x4 - x2 * y2 + y2 * y2;
            if (f > L) continue;
            if (s2647_isprime(f)) {
                if (cnt == cap) {
                    cap *= 2;
                    buf = (uint64_t *)realloc(buf, cap * sizeof(uint64_t));
                }
                buf[cnt++] = f;
            }
        }
    }
    qsort(buf, cnt, sizeof(uint64_t), s2647_cmp);
    size_t u = 0, i;
    for (i = 0; i < cnt; i++)
        if (u == 0 || buf[i] != buf[u - 1]) buf[u++] = buf[i];
    free(s2647_list);
    s2647_list = buf;
    s2647_count = u;
    s2647_limit = L;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    uint64_t L = s2647_limit ? s2647_limit : ((uint64_t)1 << 20);
    if (s2647_limit == 0) s2647_build(L);
    while ((idx_t)s2647_count < n) {
        L *= 4;
        s2647_build(L);
    }
    return (seq_t)s2647_list[n - 1];
}
