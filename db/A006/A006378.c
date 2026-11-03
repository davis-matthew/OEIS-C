/*
 * A006378  Prime self (or Colombian) numbers: primes not expressible as the
 *          sum of an integer and its digit sum.
 * https://oeis.org/A006378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 588, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 9937 (2026-11-03)
 * Helpers:   digit_sum_u64, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: primes p such that no k < p has k + digitsum(k) = p (checked over k >= p - 9*digits(p)). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_sum_u64.h"
#include "../utils/is_prime_u64.h"

static bool ds_is_self(uint64_t p) {
    uint64_t digits = 0, t = p;
    while (t > 0) {
        digits++;
        t /= 10;
    }
    uint64_t maxs = 9 * digits;
    uint64_t start = (p > maxs) ? (p - maxs) : 0;
    for (uint64_t k = start; k < p; k++) {
        if (k + digit_sum_u64(k) == p) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    static uint64_t *cache = NULL;
    static idx_t cap = 0;
    static idx_t size = 0;
    static uint64_t next_cand = 2;

    if (n == 0) return (seq_t)0;

    if (n > cap) {
        idx_t newcap = (n > 2 * cap) ? n + 64 : 2 * cap;
        uint64_t *p = (uint64_t *)realloc(cache, (size_t)newcap * sizeof(uint64_t));
        if (p == NULL) return (seq_t)0;
        cache = p;
        cap = newcap;
    }

    while (size < n) {
        uint64_t c = next_cand++;
        if (is_prime_u64(c) && ds_is_self(c)) {
            cache[size++] = c;
        }
    }

    return (seq_t)cache[n - 1];
}
