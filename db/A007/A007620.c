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
 * Helpers:   a007620_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/a007620_ok.h"

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
