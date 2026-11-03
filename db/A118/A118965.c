/*
 * A118965  Number of missing residues in Fibonacci sequence mod n.
 * https://oeis.org/A118965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 287, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 8811 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A118965: Number of missing residues in Fibonacci sequence mod n
   Formula: a(n) = n - (number of distinct residues in one Pisano period of n) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd_u64.h"

static uint64_t _lcm_uint64(uint64_t a, uint64_t b) {
    return (a / gcd_u64(a, b)) * b;
}

/* Pisano period of a prime p by brute force search.
   The period always occurs at a = 0, b = 1. */
static uint64_t _pisano_period_prime(uint64_t p) {
    if (p == 2) return 3;
    if (p == 5) return 20;
    uint64_t a = 0;
    uint64_t b = 1 % p;
    for (uint64_t i = 1; ; ++i) {
        uint64_t c = (a + b) % p;
        a = b;
        b = c;
        if (a == 0 && b == 1) return i;
    }
}

/* Pisano period of a prime power p^e (e >= 1). */
static uint64_t _pisano_period_prime_power(uint64_t p, unsigned e) {
    uint64_t base_period = _pisano_period_prime(p);
    if (p == 2) {
        if (e == 1) return 3;
        if (e == 2) return 6;
        /* e >= 3 */
        uint64_t mult = 1;
        for (unsigned i = 0; i < e - 1; ++i) mult *= 2;
        return 3 * mult;   /* 3 * 2^(e-1) */
    }
    if (p == 5) {
        uint64_t mult = 1;
        for (unsigned i = 1; i < e; ++i) mult *= 5;
        return 20 * mult;   /* 20 * 5^(e-1) */
    }
    /* general case for odd prime != 5 */
    uint64_t mult = 1;
    for (unsigned i = 1; i < e; ++i) mult *= p;
    return base_period * mult;
}

/* Pisano period of n (n >= 1) using factorisation into prime powers. */
static uint64_t _pisano_period(uint64_t n) {
    if (n <= 1) return 1;
    uint64_t period = 1;
    uint64_t m = n;
    for (uint64_t d = 2; d * d <= m; ++d) {
        if (m % d == 0) {
            unsigned exp = 0;
            while (m % d == 0) { m /= d; ++exp; }
            uint64_t pp = _pisano_period_prime_power(d, exp);
            period = _lcm_uint64(period, pp);
        }
    }
    if (m > 1) {
        /* remaining prime factor */
        uint64_t pp = _pisano_period_prime_power(m, 1);
        period = _lcm_uint64(period, pp);
    }
    return period;
}

/* gcd and lcm for uint64_t, needed for lcm of periods */

/* Main sequence function */
seq_t a(idx_t n) {
    uint64_t nn = (uint64_t)n;          /* cast to uint64_t for arithmetic */
    if (nn <= 1) return (seq_t)0;       /* a(1)=0 */

    uint64_t period = _pisano_period(nn);

    /* Allocate array for seen residues */
    uint8_t *seen = (uint8_t*)calloc((size_t)nn, 1);
    if (!seen) abort();                /* memory allocation failed */

    uint64_t a_f = 0;
    uint64_t b_f = 1 % nn;
    unsigned count = 0;

    for (uint64_t i = 0; i < period; ++i) {
        if (!seen[a_f]) {
            seen[a_f] = 1;
            ++count;
        }
        uint64_t next = (a_f + b_f) % nn;
        a_f = b_f;
        b_f = next;
    }

    free(seen);

    uint64_t missing = nn - count;
    return (seq_t)missing;
}
