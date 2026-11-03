/*
 * A007620  Numbers m such that every k <= m is a sum of proper divisors of m
 *          (for m>1).
 * https://oeis.org/A007620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   search. a(n) is the n-th m with m = 1 or m practical (Stewart's
 *            criterion: m = 2^e1*p2^e2*..., p_{i+1} <= 1 +
 *            sigma(p1^e1*...*pi^ei)) and m not a power of 2 (powers of 2 are
 *            almost-perfect, so their proper divisors sum to only m-1)
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 80 terms through n = 910 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static bool a007620_ok(uint64_t m) {
    if (m == 1) return true;
    if (m & 1) return false;
    if ((m & (m - 1)) == 0) return false;
    uint64_t x = m, s = 1;
    for (uint64_t p = 2; p * p <= x; p++) {
        if (x % p == 0) {
            if (p > s + 1) return false;
            uint64_t pk = 1, sum = 1;
            while (x % p == 0) {
                x /= p;
                pk *= p;
                sum += pk;
            }
            s *= sum;
        }
    }
    if (x > 1 && x > s + 1) return false;
    return true;
}

static uint64_t *a007620_terms = NULL;
static idx_t a007620_cnt = 0, a007620_cap = 0;
static uint64_t a007620_last = 0;

seq_t a(idx_t n) {
    while (a007620_cnt < n) {
        uint64_t m = ++a007620_last;
        if (a007620_ok(m)) {
            if (a007620_cnt == a007620_cap) {
                idx_t nc = a007620_cap ? a007620_cap * 2 : 1024;
                a007620_terms = (uint64_t *)realloc(a007620_terms, nc * sizeof(uint64_t));
                a007620_cap = nc;
            }
            a007620_terms[a007620_cnt++] = m;
        }
    }
    return (seq_t)a007620_terms[n - 1];
}
