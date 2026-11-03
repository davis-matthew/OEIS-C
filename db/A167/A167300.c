/*
 * A167300  Totally multiplicative sequence with a(p) = 8*(p-2) for prime p.
 * https://oeis.org/A167300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) is multiplicative, a(p^e) = (8*(p-2))^e; a(2k)=0
 * Range:     u8 n <= 20, u16 n <= 374, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 82 terms through n = 962 (2026-11-03)
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
    /* n = 1 => 1 by definition */
    if (n == 1) {
        return 1;
    }
    /* Prime 2 zeroes all even numbers */
    if ((n & 1) == 0) {
        return 0;
    }

    uint64_t m = (uint64_t)n;
    seq_t result = 1;

    while (m > 1) {
        uint64_t p = smallest_prime_factor(m);
        int exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        /* 8*(p-2) = 8*p - 16 */
        seq_t base = (seq_t)8 * (seq_t)p - (seq_t)16;
        seq_t pow_val = ipow(base, (seq_t)exp);
        result *= pow_val;
    }

    return result;
}
