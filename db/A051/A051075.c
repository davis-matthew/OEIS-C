/*
 * A051075  Primes p such that x^12 = -2 has a solution mod p.
 * https://oeis.org/A051075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_uint64, grow_uint64, powmod_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that x^12 = -2 (mod p) has a solution. */
/* The condition is equivalent to -2 being a 12th power residue modulo p. */
/* The function generates primes incrementally and caches all qualifying primes. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd_uint64.h"
#include "../utils/grow_uint64.h"
#include "../utils/powmod_uint64.h"

/* Helper: integer GCD for uint64_t */

/* Helper: check primality using a growing list of primes (trial division) */
static bool is_prime(uint64_t n, const uint64_t *primes, size_t primes_used) {
    if (n < 2) return false;
    for (size_t i = 0; i < primes_used; ++i) {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Static state: list of all primes, list of qualifying primes */
    static uint64_t *prime_list = NULL;
    static size_t prime_used = 0, prime_cap = 0;
    static uint64_t *qual_list = NULL;
    static size_t qual_used = 0, qual_cap = 0;
    static uint64_t last_checked = 1;

    if (n == 0) return 0;  /* sequence is 1‑based; n=0 is undefined */

    size_t target = (size_t)n - 1;

    /* Ensure enough qualifying primes are generated */
    while (qual_used <= target) {
        /* Generate next prime candidate */
        uint64_t cand;
        if (prime_used == 0) {
            cand = 2;
        } else {
            cand = last_checked + 1;
            if ((cand & 1ULL) == 0) ++cand;  /* skip evens > 2 */
        }

        /* Find next prime */
        while (!is_prime(cand, prime_list, prime_used)) cand += 2;

        /* Store the new prime */
        grow_uint64(&prime_list, &prime_cap, &prime_used, prime_used + 1);
        prime_list[prime_used - 1] = cand;
        last_checked = cand;

        /* Check if cand satisfies the condition */
        bool ok = false;
        if (cand == 2) {
            ok = true;
        } else {
            uint64_t d = gcd_uint64(12ULL, cand - 1ULL);
            uint64_t exp = (cand - 1ULL) / d;
            uint64_t base = cand - 2ULL;
            if (powmod_uint64(base, exp, cand) == 1ULL) ok = true;
        }

        if (ok) {
            grow_uint64(&qual_list, &qual_cap, &qual_used, qual_used + 1);
            qual_list[qual_used - 1] = cand;
        }
    }

    return (seq_t)qual_list[target];
}
