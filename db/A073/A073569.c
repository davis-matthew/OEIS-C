/*
 * A073569  Numbers k such that A002034(k) = A066272(k).
 * https://oeis.org/A073569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A002034(k) = A066272(k), by incremental
 *            search; A066272(k) = d(2k-1) + d(2k+1) + d_odd(k) - 5 for k >= 2
 * Range:     u8 n <= 9, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A073569_numdiv(uint64_t x) {
    uint64_t r = 1;
    for (uint64_t p = 2; p * p <= x; p++) {
        if (x % p == 0) {
            uint64_t e = 0;
            while (x % p == 0) { x /= p; e++; }
            r *= (e + 1);
        }
    }
    if (x > 1) r *= 2;
    return r;
}

static int A073569_test(uint64_t k) {
    /* Factor k: compute Kempner value and number of odd divisors */
    uint64_t x = k, kemp = 1, oddd = 1;
    for (uint64_t p = 2; p * p <= x; p++) {
        if (x % p == 0) {
            uint64_t e = 0;
            while (x % p == 0) { x /= p; e++; }
            uint64_t m = 0, cnt = 0;
            while (cnt < e) {
                m += p;
                uint64_t t = m;
                while (t % p == 0) { cnt++; t /= p; }
            }
            if (m > kemp) kemp = m;
            if (p != 2) oddd *= (e + 1);
        }
    }
    if (x > 1) {
        if (x > kemp) kemp = x;
        if (x != 2) oddd *= 2;
    }
    uint64_t anti = A073569_numdiv(2 * k - 1) + A073569_numdiv(2 * k + 1) + oddd - 5;
    return anti == kemp;
}

static uint64_t *A073569_terms = 0;
static uint64_t A073569_cnt = 0, A073569_cap = 0, A073569_next = 2;

seq_t a(idx_t n) {
    while (A073569_cnt < (uint64_t)n) {
        uint64_t k = A073569_next++;
        if (A073569_test(k)) {
            if (A073569_cnt == A073569_cap) {
                A073569_cap = A073569_cap ? A073569_cap * 2 : 256;
                A073569_terms = (uint64_t *)realloc(A073569_terms, A073569_cap * sizeof(uint64_t));
            }
            A073569_terms[A073569_cnt++] = k;
        }
    }
    return (seq_t)A073569_terms[n - 1];
}
