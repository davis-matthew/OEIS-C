/*
 * A002646  Half-quartan primes: primes of the form p = (x^4 + y^4)/2.
 * https://oeis.org/A002646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   search: a(n) is the n-th smallest prime of the form (x^4 +
 *            y^4)/2 with x > y >= 1, x and y odd. The code lists these primes
 *            up to a bound, sorts them, and doubles the bound until there are
 *            at least n
 * Range:     u8 n <= 1, u16 n <= 21, u32 n <= 2358, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   hq_cmp, hq_isprime, hq_mulmod, hq_powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/hq_cmp.h"
#include "../utils/hq_isprime.h"
#include "../utils/hq_mulmod.h"
#include "../utils/hq_powmod.h"

static uint64_t *hq_list = NULL;
static size_t hq_cnt = 0;
static uint64_t hq_bound = 0;

static void hq_build(uint64_t B) {
    size_t cap = 1024, cnt = 0;
    uint64_t *v = (uint64_t *)malloc(cap * sizeof(uint64_t));
    uint64_t x, y;
    for (x = 3;; x += 2) {
        uint64_t x4 = x * x * x * x;
        if ((x4 + 1) / 2 > B) break;
        for (y = 1; y < x; y += 2) {
            uint64_t y4 = y * y * y * y;
            uint64_t p = (x4 + y4) / 2;
            if (p > B) break;
            if (hq_isprime(p)) {
                if (cnt == cap) {
                    cap *= 2;
                    v = (uint64_t *)realloc(v, cap * sizeof(uint64_t));
                }
                v[cnt++] = p;
            }
        }
    }
    qsort(v, cnt, sizeof(uint64_t), hq_cmp);
    free(hq_list);
    hq_list = v;
    hq_cnt = cnt;
    hq_bound = B;
}

seq_t a(idx_t n) {
    uint64_t B = hq_bound ? hq_bound : 100000;
    if (hq_list == NULL) hq_build(B);
    while ((idx_t)hq_cnt < n) {
        B *= 4;
        hq_build(B);
    }
    return (seq_t)hq_list[n - 1];
}
