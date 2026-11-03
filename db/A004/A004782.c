/*
 * A004782  Numbers k such that 2*(2k-3)!/(k!*(k-1)!) is an integer.
 * https://oeis.org/A004782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over k >= 2 for those where m = k-1 divides the Catalan
 *            number C(m) (a(n) = A014847(n) + 1). For each prime power p^e ||
 *            m, Kummer's theorem gives v_p(C(m)) = number of carries when m+m
 *            is added in base p (p does not divide m+1), so m | C(m) exactly
 *            when every such carry count is >= e
 * Range:     u8 n <= 29, u16 n <= 7458, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9567 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t carries2(uint64_t x, uint64_t p) {
    uint64_t cnt = 0, carry = 0;
    while (x) {
        uint64_t d = x % p;
        uint64_t s = 2 * d + carry;
        carry = (s >= p) ? 1 : 0;
        cnt += carry;
        x /= p;
    }
    return cnt;
}

static bool divides_catalan(uint64_t m) {
    uint64_t x = m, p;
    for (p = 2; p * p <= x; p++) {
        if (x % p == 0) {
            uint64_t e = 0;
            while (x % p == 0) { x /= p; e++; }
            if (carries2(m, p) < e) return false;
        }
    }
    if (x > 1) {
        if (carries2(m, x) < 1) return false;
    }
    return true;
}

static uint64_t *terms = NULL;
static idx_t nterms = 0, cap = 0;
static uint64_t next_k = 2;

seq_t a(idx_t n) {
    while (nterms < n) {
        uint64_t k = next_k++;
        if (divides_catalan(k - 1)) {
            if (nterms == cap) {
                idx_t nc = cap ? cap * 2 : 1024;
                terms = (uint64_t *)realloc(terms, nc * sizeof(uint64_t));
                cap = nc;
            }
            terms[nterms++] = k;
        }
    }
    return (seq_t)terms[n - 1];
}
