/*
 * A002202  Values taken by totient function phi(m) (A000010).
 * https://oeis.org/A002202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   search: a(n) is the n-th v >= 1 with v = phi(m) for some m. Each
 *            v is tested by recursively building the inverse totient (v =
 *            prod p^(k-1)*(p-1) over distinct primes p, with p-1 | v)
 * Range:     u8 n <= 87, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a2202_isprime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/a2202_isprime.h"

static int a2202_is_tot(uint64_t v, uint64_t pmax);

static int a2202_try(uint64_t v, uint64_t q, uint64_t pmax) {
    uint64_t p = q + 1, r;
    if (p >= pmax || !a2202_isprime(p)) return 0;
    r = v / q;
    for (;;) {
        if (a2202_is_tot(r, p)) return 1;
        if (r % p != 0) break;
        r /= p;
    }
    return 0;
}

/* true if v = phi(m) with m built only from primes < pmax */
static int a2202_is_tot(uint64_t v, uint64_t pmax) {
    uint64_t d;
    if (v == 1) return 1;
    if (v & 1) return 0;
    for (d = 1; d * d <= v; d++) {
        if (v % d == 0) {
            if (a2202_try(v, d, pmax)) return 1;
            if (d * d != v && a2202_try(v, v / d, pmax)) return 1;
        }
    }
    return 0;
}

static uint64_t *a2202_terms = NULL;
static size_t a2202_cnt = 0, a2202_cap = 0;
static uint64_t a2202_next = 1;

seq_t a(idx_t n) {
    while (a2202_cnt < (size_t)n) {
        uint64_t v = a2202_next++;
        if (a2202_is_tot(v, UINT64_MAX)) {
            if (a2202_cnt == a2202_cap) {
                size_t nc = a2202_cap ? 2 * a2202_cap : 1024;
                uint64_t *t = (uint64_t *)realloc(a2202_terms, nc * sizeof(uint64_t));
                if (!t) return 0;
                a2202_terms = t;
                a2202_cap = nc;
            }
            a2202_terms[a2202_cnt++] = v;
        }
    }
    return (seq_t)a2202_terms[n - 1];
}
