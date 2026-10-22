/*
 * A006488  Numbers n such that n! has a square number of digits.
 * https://oeis.org/A006488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search. Let D(k) = floor(log10(k!)) + 1 be the number of digits
 *            of k!. For k <= 20, D(k) is counted exactly from 64-bit
 *            factorials. For larger k, each square q^2 is taken in turn, the
 *            smallest k with D(k) >= q^2 is found, and k is a term when D(k)
 *            == q^2. Stirling's series in long double gives lo...
 * Range:     u8 n <= 13, u16 n <= 146, u32 n <= 1311, u64 n <= 1311,
 *            u128 n <= 1311, big any n
 * Helpers:   a000037_isqrt, log10fact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/a000037_isqrt.h"
#include "../utils/log10fact.h"

static uint64_t *terms = NULL;
static idx_t cnt = 0, cap = 0;
static uint64_t nextq = 5, lastk = 20;

static void push_term(uint64_t k) {
    if (cnt == cap) {
        idx_t nc = cap ? cap * 2 : 64;
        terms = (uint64_t *)realloc(terms, nc * sizeof(uint64_t));
        cap = nc;
    }
    terms[cnt++] = k;
}

static void init_small(void) {
    uint64_t f = 1;
    uint64_t k;
    for (k = 0; k <= 20; k++) {
        if (k > 0) f *= k;
        uint64_t d = 0, t = f;
        while (t > 0) { d++; t /= 10; }
        uint64_t r = a000037_isqrt(d);
        if (r * r == d) push_term(k);
    }
}

seq_t a(idx_t n) {
    if (cnt == 0) init_small();
    while (cnt < n) {
        uint64_t q = nextq++;
        long double T = (long double)q * (long double)q - 1.0L;
        uint64_t lo = lastk + 1, hi, k;
        if (log10fact(lo) >= T) {
            k = lo;
        } else {
            uint64_t step = 1;
            hi = lo + step;
            while (log10fact(hi) < T) {
                lo = hi;
                step *= 2;
                hi = lo + step;
            }
            while (hi - lo > 1) {
                uint64_t mid = lo + (hi - lo) / 2;
                if (log10fact(mid) >= T) hi = mid; else lo = mid;
            }
            k = hi;
        }
        lastk = k - 1;
        if (log10fact(k) < T + 1.0L) push_term(k);
    }
    return (seq_t)terms[n - 1];
}
