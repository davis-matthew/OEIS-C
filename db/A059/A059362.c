/*
 * A059362  Primes p such that x^24 = 2 has no solution mod p.
 * https://oeis.org/A059362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   A059362 is the sequence of primes p such that the congruence
 *            x^24 ≡ 2 (mod p) has no solution. For a prime p the condition is
 *            2^((p-1)/gcd(p-1,24)) ≠ 1 (mod p). The code generates primes by
 *            trial division, tests the above condition using modular
 *            exponentiation, and caches the primes that pass the...
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _gcd_u64, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mod_pow.h"
#include "../utils/_gcd_u64.h"

/* integer gcd for 64‑bit values */

seq_t a(idx_t n)
{
    /* static state for memoization */
    static bool initialized = false;
    static idx_t *prime_list = NULL;
    static size_t prime_count = 0, prime_cap = 0;
    static seq_t *good_list = NULL;
    static size_t good_count = 0, good_cap = 0;
    static idx_t candidate = 3;          /* next odd number to test */

    if (!initialized) {
        /* initialise prime list with 2 */
        prime_cap = 64;
        prime_list = (idx_t *)malloc(prime_cap * sizeof(idx_t));
        prime_list[0] = 2;
        prime_count = 1;

        /* initialise good‑prime list */
        good_cap = 64;
        good_list = (seq_t *)malloc(good_cap * sizeof(seq_t));
        good_count = 0;

        initialized = true;
    }

    /* generate sufficient good primes */
    while (good_count < n) {
        /* ---- primality test for candidate ---- */
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            idx_t p = prime_list[i];
            if (p * p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* store the new prime */
            if (prime_count == prime_cap) {
                prime_cap <<= 1;
                prime_list = (idx_t *)realloc(prime_list,
                                              prime_cap * sizeof(idx_t));
            }
            prime_list[prime_count++] = candidate;

            /* test the 24‑th‑power‑residue condition */
            uint64_t p_minus_1 = (uint64_t)(candidate - 1);
            uint64_t g = _gcd_u64(24ULL, p_minus_1);
            uint64_t exp = p_minus_1 / g;       /* exponent to test */
            seq_t res = mod_pow((seq_t)2, exp, (seq_t)candidate);

            if (res != (seq_t)1) {               /* 2 is NOT a 24‑th residue */
                if (good_count == good_cap) {
                    good_cap <<= 1;
                    good_list = (seq_t *)realloc(good_list,
                                                 good_cap * sizeof(seq_t));
                }
                good_list[good_count++] = (seq_t)candidate;
            }
        }
        candidate += 2;            /* next odd number */
    }

    return good_list[n - 1];
}
