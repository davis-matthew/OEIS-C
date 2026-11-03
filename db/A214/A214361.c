/*
 * A214361  Expansion of c(q^2) * (c(q) + 2 * c(q^4)) / 9 in powers of q where
 *          c() is a cubic AGM theta function.
 * https://oeis.org/A214361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   multiplicative definition from OEIS A214361 * * a(1) = 1 *
 *            a(2^e) = 3 (e > 0) * a(3^e) = 3^e * a(p^e) = (p^(e+1) – 1) / (p
 *            – 1) for primes p > 3 * * The function factors n using the
 *            provided helper * uint64_t smallest_prime_factor(uint64_t n) *
 *            (declared static in the header) and then evaluates e...
 * Range:     u8 n <= 16, u16 n <= 586, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 84 terms through n = 975 (2026-11-03)
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
    if (n == (idx_t)1) return (seq_t)1;            /* offset 1 */

    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;

    while (m > 1) {
        uint64_t p = smallest_prime_factor(m);    /* smallest prime factor */
        uint64_t exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }

        seq_t part;
        if (p == 2) {
            part = (seq_t)3;                       /* a(2^e) = 3 */
        } else if (p == 3) {
            part = ipow((seq_t)3, (seq_t)exp);  /* a(3^e) = 3^e */
        } else {
            seq_t top = ipow((seq_t)p, (seq_t)(exp + 1));
            seq_t bottom = (seq_t)p - (seq_t)1;
            part = (top - (seq_t)1) / bottom;      /* (p^(e+1)-1)/(p-1) */
        }

        result *= part;
    }
    return result;
}
