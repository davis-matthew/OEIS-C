/*
 * A001912  Numbers k such that 4*k^2 + 1 is prime.
 * https://oeis.org/A001912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n-th k >= 1 such that 4*k^2 + 1 is prime (found by
 *            searching k in order, with a Miller-Rabin primality test)
 * Range:     u8 n <= 69, u16 n <= 8417, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9850 (2026-11-03)
 * Helpers:   is_prime_u64, mulmod, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod.h"
#include "../utils/powmod_u64.h"

static uint64_t *a001912_terms = NULL;
static idx_t a001912_cnt = 0;
static idx_t a001912_cap = 0;
static uint64_t a001912_lastk = 0;

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    while (a001912_cnt < n) {
        uint64_t k = ++a001912_lastk;
        if (is_prime_u64(4 * k * k + 1)) {
            if (a001912_cnt == a001912_cap) {
                idx_t nc = a001912_cap ? a001912_cap * 2 : 1024;
                uint64_t *p = (uint64_t *)realloc(a001912_terms, (size_t)nc * sizeof(uint64_t));
                if (!p) return (seq_t)0;
                a001912_terms = p;
                a001912_cap = nc;
            }
            a001912_terms[a001912_cnt++] = k;
        }
    }
    return (seq_t)a001912_terms[n - 1];
}
