/*
 * A115979  Expansion of (1 - theta_4(q)*theta_4(q^3))/2 in powers of q.
 * https://oeis.org/A115979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) is multiplicative with a(2^e) = -3*(1+(-1)^e)/2 ( e>0 )
 *            a(3^e) = 1 a(p^e) = 1+e if p ≡ 1 (mod 6) a(p^e) = (1+(-1)^e)/2
 *            if p ≡ 5 (mod 6)
 * Range:     i8 n <= 65537, i16 n <= 65537, i32 n <= 65537, i64 n <= 65537,
 *            i128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8734 (2026-11-03)
 * Helpers:   gcd_u64, is_prime_u64, mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: value contributed by a prime power p^e. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/gcd_u64.h"
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

static seq_t prime_power_value(uint64_t p, int exp)
{
    if (p == 2) {
        /* -3 * (1 + (-1)^e) / 2   → 0 for odd e, -3 for even e */
        return (exp & 1) ? (seq_t)0 : (seq_t)-3;
    } else if (p == 3) {
        return (seq_t)1;
    } else if (p % 6 == 1) {
        return (seq_t)(1 + exp);
    } else { /* p % 6 == 5 */
        return (exp & 1) ? (seq_t)0 : (seq_t)1;
    }
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;

    while (m > 1) {
        uint64_t p = smallest_prime_factor(m);
        if (p == 0) p = m;           /* fallback: m is prime */

        int exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }

        seq_t f = prime_power_value(p, exp);
        if (f == (seq_t)0) return (seq_t)0;   /* product becomes zero */
        result *= f;
    }

    return result;
}
