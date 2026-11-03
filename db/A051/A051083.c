/*
 * A051083  Primes p such that x^28 = -2 has a solution mod p.
 * https://oeis.org/A051083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 975 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes p for which x^28 ≡ –2 (mod p) has a solution.
 * The function sieve for successively larger limits to find the n‑th such prime.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>
#include "../utils/gcd_u64.h"

/* Estimate an upper bound for the n‑th good prime.
 * Use n*(log n + log log n) / density, with a safety factor.
 */
static uint64_t upper_bound_for_ngoodprime(uint64_t n)
{
    if (n < 10) return 50;          /* small n → trivial limit */
    double dn = (double)n;
    double estimate = dn * (log(dn) + log(log(dn)));
    /* Density of good primes ≈ 0.3, so inflate by 1.6 for safety. */
    estimate *= 1.6;
    return (uint64_t)(estimate) + 10;
}

/* Greatest common divisor (uint64_t). */

/* Modular exponentiation (uint64_t). */
static uint64_t modpow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    __uint128_t result = 1;
    __uint128_t cur = base % mod;
    while (exp) {
        if (exp & 1ULL)
            result = (result * cur) % mod;
        cur = (cur * cur) % mod;
        exp >>= 1;
    }
    return (uint64_t)result;
}

/* True iff there exists x with x^28 ≡ –2 (mod p).  p must be prime. */
static bool has_solution_x28_minus2(uint64_t p)
{
    if (p == 2) return true;          /* 0^28 ≡ –2 (mod 2) */
    uint64_t g   = gcd_u64(28ULL, p - 1);
    uint64_t exp = (p - 1) / g;
    uint64_t base = (p - 2) % p; /* –2 modulo p */
    return modpow64(base, exp, p) == 1ULL;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    uint64_t target = (uint64_t)n;
    uint64_t limit = upper_bound_for_ngoodprime(target);

    while (true) {
        /* Allocate sieve array. */
        uint8_t *is_prime = (uint8_t*)malloc(limit + 1);
        if (!is_prime) abort();
        for (uint64_t i = 0; i <= limit; ++i) is_prime[i] = 1;
        is_prime[0] = is_prime[1] = 0;
        for (uint64_t p = 2; p * p <= limit; ++p)
            if (is_prime[p])
                for (uint64_t q = p * p; q <= limit; q += p)
                    is_prime[q] = 0;

        uint64_t count = 0;
        for (uint64_t p = 2; p <= limit; ++p)
            if (is_prime[p]) {
                if (has_solution_x28_minus2(p)) {
                    ++count;
                    if (count == target) {
                        free(is_prime);
                        return (seq_t)p;
                    }
                }
            }
        free(is_prime);

        /* Not enough good primes: increase limit and retry. */
        limit *= 2;
    }
}
