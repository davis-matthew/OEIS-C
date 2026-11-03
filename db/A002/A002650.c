/*
 * A002650  Quintan primes: p = (x^5 + y^5)/(x + y).
 * https://oeis.org/A002650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) is the n-th smallest prime of the form x^4 - x^3*y
 *            + x^2*y^2 - x*y^3 + y^4 = (x^5 + y^5)/(x + y), x > y >= 1. All
 *            pairs whose value is at most a limit L are listed and tested
 *            with deterministic Miller-Rabin, then the primes are sorted and
 *            deduplicated. L is doubled until there are at...
 * Range:     u8 n <= 3, u16 n <= 26, u32 n <= 3649, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 53 terms through n = 9424 (2026-11-03)
 * Helpers:   is_prime_u64, mulmod, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod.h"
#include "../utils/powmod_u64.h"

static int q2650_cmp(const void *p, const void *q) {
    uint64_t x = *(const uint64_t *)p, y = *(const uint64_t *)q;
    return (x > y) - (x < y);
}

static unsigned __int128 q2650_f(uint64_t x, uint64_t y) {
    unsigned __int128 X = x, Y = y;
    return (X * X * X * X * X + Y * Y * Y * Y * Y) / (X + Y);
}

static uint64_t *q2650_list = NULL;
static size_t q2650_cnt = 0;

static void q2650_build(uint64_t L) {
    size_t cap = 1024, cnt = 0;
    uint64_t *v = (uint64_t *)malloc(cap * sizeof(uint64_t));
    uint64_t x, y;
    size_t i, u;
    for (y = 1; q2650_f(y + 1, y) <= L; y++) {
        for (x = y + 1;; x++) {
            unsigned __int128 fv = q2650_f(x, y);
            if (fv > L) break;
            if (is_prime_u64((uint64_t)fv)) {
                if (cnt == cap) {
                    cap *= 2;
                    v = (uint64_t *)realloc(v, cap * sizeof(uint64_t));
                }
                v[cnt++] = (uint64_t)fv;
            }
        }
    }
    qsort(v, cnt, sizeof(uint64_t), q2650_cmp);
    u = 0;
    for (i = 0; i < cnt; i++)
        if (u == 0 || v[i] != v[u - 1]) v[u++] = v[i];
    free(q2650_list);
    q2650_list = v;
    q2650_cnt = u;
}

seq_t a(idx_t n) {
    static uint64_t L = 0;
    if (L == 0) { L = 1u << 16; q2650_build(L); }
    while (q2650_cnt < n) {
        L *= 4;
        q2650_build(L);
    }
    return (seq_t)q2650_list[n - 1];
}
