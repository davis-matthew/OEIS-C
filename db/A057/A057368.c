/*
 * A057368  Number of Gaussian primes (in the first half-quadrant; i.e., 0 to
 *          45 degrees) with real part = n.
 * https://oeis.org/A057368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = A069004(n) + 1 if n = 1 OR n is prime ≡ 3 (mod 4) *
 *            A069004(n) otherwise * A069004(n) = # { b | 1 ≤ b < n and n^2 +
 *            b^2 is prime } (real part = n, * imaginary part < n, 0 < b)
 *            (first half‑quadrant) * * The implementation uses a
 *            deterministic Miller–Rabin test for 64‑bit * integers and co...
 * Range:     u8 n <= 1884, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_mul_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mod_mul_u64.h"

/* 64‑bit modular multiplication without overflow (uses __uint128_t). */

/* 64‑bit modular exponentiation (uses __uint128_t). */
static uint64_t mod_pow_u64(uint64_t base, uint64_t exp, uint64_t mod) {
    __uint128_t result = 1;
    __uint128_t b = base % mod;
    while (exp) {
        if (exp & 1) result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return (uint64_t)result;
}

/* Deterministic Miller–Rabin primality test for 64‑bit integers. */
static bool is_prime_u64(uint64_t n) {
    if (n < 2) return false;

    /* Trial division by small primes. */
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL,
        17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    /* Miller–Rabin with known witnesses for 64 bits. */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;          /* base ≡ 0 mod n ⇒ skip */
        uint64_t x = mod_pow_u64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (unsigned r = 1; r < s; ++r) {
            x = mod_mul_u64(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    /* The index starts from 1 (offset 1). */
    uint64_t n64 = (uint64_t)n;
    uint64_t count = 0;

    /* Count Gaussian primes with imaginary part 0 < b < n. */
    for (uint64_t b = 1; b < n64; ++b) {
        __uint128_t val128 = (__uint128_t)n64 * n64 + (__uint128_t)b * b;
        uint64_t val = (uint64_t)val128;
        if (is_prime_u64(val)) ++count;
    }

    /* Add the real‑axis prime if applicable. */
    if (n64 == 1 || (is_prime_u64(n64) && (n64 & 3ULL) == 3ULL))
        ++count;

    return (seq_t)count;
}
