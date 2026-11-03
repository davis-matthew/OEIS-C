/*
 * A002174  Values taken by reduced totient function psi(n).
 * https://oeis.org/A002174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   search. v is a term iff v = lcm of all lambda(p^k) that divide
 *            v, where lambda(2)=1, lambda(4)=2, lambda(2^k)=2^(k-2) for k>=3,
 *            and lambda(p^k)=p^(k-1)*(p-1) for odd primes p. Terms are listed
 *            in increasing order
 * Range:     u8 n <= 92, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9843 (2026-11-03)
 * Helpers:   gcd_u64, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_u64.h"
#include "../utils/is_prime_u64.h"

static uint64_t a2174_contrib(uint64_t v, uint64_t d) {
    uint64_t p = d + 1;
    if (!is_prime_u64(p)) return 1;
    if (p == 2) {
        uint64_t c = 1, w = v;
        while (w % 2 == 0) { c *= 2; w /= 2; }
        return c;
    }
    uint64_t c = d;
    while ((v / c) % p == 0) c *= p;
    return c;
}

static bool a2174_ok(uint64_t v) {
    if (v == 1) return true;
    if (v & 1) return false;
    uint64_t L = 1;
    for (uint64_t d = 1; d * d <= v; d++) {
        if (v % d) continue;
        uint64_t e = v / d;
        uint64_t c = a2174_contrib(v, d);
        L = L / gcd_u64(L, c) * c;
        if (e != d) {
            c = a2174_contrib(v, e);
            L = L / gcd_u64(L, c) * c;
        }
        if (L == v) return true;
    }
    return L == v;
}

static uint64_t *a2174_terms = NULL;
static idx_t a2174_cnt = 0, a2174_cap = 0;
static uint64_t a2174_next = 1;

seq_t a(idx_t n) {
    while (a2174_cnt < n) {
        if (a2174_ok(a2174_next)) {
            if (a2174_cnt == a2174_cap) {
                idx_t nc = a2174_cap ? a2174_cap * 2 : 1024;
                a2174_terms = (uint64_t *)realloc(a2174_terms, sizeof(uint64_t) * nc);
                a2174_cap = nc;
            }
            a2174_terms[a2174_cnt++] = a2174_next;
        }
        a2174_next++;
    }
    return (seq_t)a2174_terms[n - 1];
}
