/*
 * A096172  Largest prime factor of n^4 + 1.
 * https://oeis.org/A096172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime factor of n^4 + 1 (OEIS A096172)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 265, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_uint64, mod_mul_uint64, pollards_rho
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd_uint64.h"
#include "../utils/mod_mul_uint64.h"
#include "../utils/pollards_rho.h"

/* GCD for 64‑bit integers */

/* Modular multiplication avoiding overflow (used only for 64‑bit numbers) */

/* Modular exponentiation */
static uint64_t mod_pow_uint64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mod_mul_uint64(r, a, mod);
        a = mod_mul_uint64(a, a, mod);
        e >>= 1;
    }
    return r;
}

/* Deterministic Miller–Rabin primality test for 64‑bit integers */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof small_primes / sizeof *small_primes; ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }

    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    static const uint64_t witnesses[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof witnesses / sizeof *witnesses; ++i) {
        uint64_t a = witnesses[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul_uint64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Recursive factorisation keeping track of the largest prime factor */
static void factor_uint64(uint64_t n, uint64_t *max_p)
{
    if (n <= 1) return;
    if (is_prime_uint64(n)) {
        if (n > *max_p) *max_p = n;
        return;
    }
    uint64_t d = pollards_rho(n);
    factor_uint64(d, max_p);
    factor_uint64(n / d, max_p);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* n is >= 1 as per the OEIS offset */
    uint64_t nn = (uint64_t)n;
    uint64_t sq = nn * nn;         /* n^2 fits in 64 bits for n <= 10^4 */
    uint64_t val = sq * sq + 1ULL; /* n^4 + 1, safe for n <= 10^4 */

    uint64_t max_prime = 1ULL;
    factor_uint64(val, &max_prime);
    return (seq_t)max_prime;
}
