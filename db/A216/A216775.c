/*
 * A216775  Primes p such that x^60 = -2 has no solution mod p.
 * https://oeis.org/A216775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that x^60 = -2 has no solution mod
 *            p
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   test_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/test_prime.h"

/* Helper function: modular multiplication using 128-bit to avoid overflow */
static uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t mod) {
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

/* Helper function: modular exponentiation */
static uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            result = mod_mul(result, base, mod);
        base = mod_mul(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* Simple GCD for uint64_t */
static uint64_t gcd_uint64(uint64_t a, uint64_t b) {
    while (b) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Generate enough primes and qualifying primes up to requirement */
static void ensure_primes(idx_t n, uint64_t **qual_ptr, size_t *qual_count) {
    static uint64_t *primes = NULL;
    static size_t primes_size = 0;
    static size_t primes_count = 0;
    static bool built = false;

    if (!built) {
        /* starting prime list: 2,3 */
        primes_size = 64;
        primes = (uint64_t*)malloc(primes_size * sizeof(uint64_t));
        primes[0] = 2;
        primes[1] = 3;
        primes_count = 2;
        built = true;
    }

    /* Grow primes list if needed */
    static uint64_t candidate = 5;
    while (*qual_count < (size_t)n) {
        /* test candidate for primality */
        if (test_prime(candidate, primes, primes_count)) {
            /* store prime */
            if (primes_count >= primes_size) {
                primes_size *= 2;
                primes = (uint64_t*)realloc(primes, primes_size * sizeof(uint64_t));
            }
            primes[primes_count++] = candidate;

            /* check if it satisfies condition */
            if (candidate >= 5) {
                uint64_t d = gcd_uint64(60ULL, candidate - 1);
                uint64_t e = (candidate - 1) / d;
                uint64_t base = (candidate + candidate - 2) % candidate; /* -2 mod candidate */
                uint64_t res = mod_pow(base, e, candidate);
                if (res != 1) {
                    *qual_ptr = (uint64_t*)realloc(*qual_ptr, (*qual_count + 1) * sizeof(uint64_t));
                    (*qual_ptr)[*qual_count] = candidate;
                    (*qual_count)++;
                }
            }
        }
        candidate += 2; /* skip even numbers */
    }
}

seq_t a(idx_t n) {
    static uint64_t *qualities = NULL;
    static size_t known = 0;

    if (n < 1) return 0; /* undefined for n<1, but not expected */
    if (known < (size_t)n) {
        ensure_primes(n, &qualities, &known);
    }
    return (seq_t)qualities[n - 1];
}
