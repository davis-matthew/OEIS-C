/*
 * A154321  Primes p such that p, p^2 + 2p - 4 and p^2 + 8p - 4 are prime.
 * https://oeis.org/A154321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 133, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th prime p such that
   p, p² + 2p – 4 and p² + 8p – 4 are all prime. */

/* Minor helpers */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* modular multiplication modulo n (using __uint128_t for safety) */
static inline uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

/* modular exponentiation modulo n */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1)
            result = mul_mod_uint64(result, base, mod);
        base = mul_mod_uint64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* Deterministic Miller‑Rabin primality test for 64‑bit integers */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] =
        {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    /* write n-1 as d * 2^s */
    uint64_t d = n - 1;
    unsigned int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* Bases that make Miller‑Rabin deterministic for 64‑bit integers */
    static const uint64_t bases[] =
        {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;  /* base divides n */
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (unsigned int r = 1; r < s; ++r) {
            x = mul_mod_uint64(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined, but safe */
    uint64_t count = 0;

    uint64_t p = 2;           /* first prime */
    while (true) {
        if (is_prime_uint64(p)) {
            uint64_t v1 = p * p + 2 * p - 4;
            uint64_t v2 = p * p + 8 * p - 4;
            if (is_prime_uint64(v1) && is_prime_uint64(v2)) {
                ++count;
                if (count == n)
                    return (seq_t)p;
            }
        }
        /* next candidate */
        if (p == 2) p = 3;
        else       p += 2;
    }
}
