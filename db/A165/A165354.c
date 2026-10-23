/*
 * A165354  Primes p such that p+(p^2-1)/8 is an integer but not a prime
 *          number.
 * https://oeis.org/A165354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: sieve primes up to a bound, test N(p)=p+(p^2-1)/8 for primality,
   collect those p for which N(p) is composite.  The n‑th such prime is returned. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/mul_mod64.h"

/* Modular multiplication (a*b) % mod for 64‑bit values using 128‑bit intermediate */

/* Modular exponentiation (base^exp) % mod for 64‑bit values */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1ULL) result = mul_mod64(result, b, mod);
        b = mul_mod64(b, b, mod);
        exp >>= 1;
    }
    return result;
}

/* Deterministic Miller–Rabin primality test for 64‑bit values */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }

    static const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL};
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a >= n) continue;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Estimate an upper bound for the n-th prime (for n ≥ 6) */
static idx_t estimate_prime_limit(idx_t n) {
    if (n < 6) return 15;
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn)));
    return (idx_t)(bound + 3.0);
}

/* Main function for the sequence */
seq_t a(idx_t n) {
    if (n < 1) return 0;

    idx_t limit = estimate_prime_limit(n);
    while (1) {
        /* Allocate sieve array */
        bool *sieve = (bool*)calloc(limit + 1, sizeof(bool));
        if (!sieve) return 0;       /* allocation failure */
        for (idx_t i = 0; i <= limit; ++i) sieve[i] = true;
        if (limit >= 0) sieve[0] = false;
        if (limit >= 1) sieve[1] = false;

        /* Eratosthenes sieve */
        for (idx_t i = 2; (idx_t)i * (idx_t)i <= limit; ++i) {
            if (sieve[i]) {
                for (idx_t j = i * i; j <= limit; j += i) sieve[j] = false;
            }
        }

        /* Scan primes and count valid terms */
        idx_t count = 0;
        seq_t result = 0;
        for (idx_t p = 3; p <= limit; p += 2) {      /* skip even numbers (p==2 handled below) */
            if (!sieve[p]) continue;
            uint64_t p64 = (uint64_t)p;
            uint64_t np = p64 + (p64 * p64 - 1ULL) / 8ULL;
            if (!is_prime_uint64(np)) {
                count++;
                if (count == n) {
                    result = (seq_t)p64;
                    free(sieve);
                    return result;
                }
            }
        }
        free(sieve);
        /* Not enough terms – increase limit and retry */
        limit *= 2;
    }
}
