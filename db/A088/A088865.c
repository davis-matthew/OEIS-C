/*
 * A088865  (Sum of distinct prime factors)^(sum of prime exponents).
 * https://oeis.org/A088865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A008472(n)^A001222(n)
 * Range:     u8 n <= 19, u16 n <= 119, u32 n <= 1311, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9840 (2026-11-03)
 * Helpers:   gcd_u64, ipow, is_prime_u64, mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/gcd_u64.h"
#include "../utils/ipow.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod.h"

/* Pollard–Rho: find a non‑trivial divisor of n (n composite) */
static uint64_t pollard(uint64_t n) {
    if (n % 2 == 0) return 2;
    uint64_t c = 1;
    while (1) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (mulmod(x, x, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            uint64_t diff = x > y ? x - y : y - x;
            d = gcd_u64(diff, n);
        }
        if (d != n) return d;
        ++c;  /* try another constant */
    }
}

/* recursive smallest prime factor */
static uint64_t smallest_prime_factor(uint64_t n) {
    if (n % 2 == 0) return 2;
    if (is_prime_u64(n)) return n;
    uint64_t d = pollard(n);
    uint64_t s1 = smallest_prime_factor(d);
    uint64_t s2 = smallest_prime_factor(n / d);
    return (s1 < s2) ? s1 : s2;
}

seq_t a(idx_t n)
{
    /* a(1) = 1 by convention (empty factorization) */
    if (n == 1) return (seq_t)1;

    uint64_t t = (uint64_t)n;
    uint64_t sum_primes = 0;
    uint64_t sum_exp    = 0;

    while (t > 1) {
        uint64_t p = smallest_prime_factor(t);
        uint64_t cnt = 0;
        while (t % p == 0) {
            t /= p;
            ++cnt;
        }
        sum_primes += p;
        sum_exp    += cnt;
    }

    return ipow((seq_t)sum_primes, (seq_t)sum_exp);
}
