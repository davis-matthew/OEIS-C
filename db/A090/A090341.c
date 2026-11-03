/*
 * A090341  Difference between the sums of the prime factors, including
 *          multiplicity, of n and those of n + 2.
 * https://oeis.org/A090341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A001414(n) - A001414(n+2)
 * Range:     i8 n <= 85, i16 n <= 85, i32 n <= 85, i64 n <= 85, i128 n <= 85,
 *            big any n
 * Validated: partial, pass, 85 terms through n = 85 (2026-11-03)
 * Helpers:   gcd_u64, is_prime_u64, mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

/* Sum of the prime factors of n, counted with multiplicity. */
static seq_t sum_prime_factors(uint64_t n)
{
    seq_t sum = 0;
    while (n > 1) {
        uint64_t p = smallest_prime_factor(n);
        sum += (seq_t)p;
        n /= p;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* n >= 1 by definition; behaviour for n < 1 is unspecified */
    seq_t sum_n   = sum_prime_factors((uint64_t)n);
    seq_t sum_np2 = sum_prime_factors((uint64_t)n + 2);
    return sum_n - sum_np2;
}
