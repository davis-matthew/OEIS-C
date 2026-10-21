/*
 * A059334  Primes p such that x^30 = 2 has no solution mod p.
 * https://oeis.org/A059334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime p for which x^30 ≡ 2 (mod p) has no
 *            solution
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_uint64, modpow_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A straightforward incremental sieve combined with the residue test. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd_uint64.h"
#include "../utils/modpow_uint64.h"

/* Greatest common divisor, for uint64_t values. */

/* Ensure a dynamic array has capacity at least 'needed'. */
static void ensure_cap(uint64_t **arr, size_t *cap, size_t needed)
{
    if (needed > *cap) {
        size_t new_cap = (*cap == 0 ? 16 : *cap * 2);
        while (new_cap < needed)
            new_cap <<= 1;
        *arr = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Persistent state across calls. */
    static uint64_t *primes       = NULL;
    static size_t   primes_cap   = 0, primes_len = 0;
    static uint64_t *special     = NULL;
    static size_t   special_cap  = 0, special_len  = 0;
    static uint64_t last_checked = 3; /* first odd prime */

    /* Obtain enough special primes to answer the request. */
    while (special_len < (size_t)n) {
        uint64_t cand = last_checked;
        /* primality test via trial division using the primes list. */
        bool is_prime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            /* Grow primes array and store the new prime. */
            ensure_cap(&primes, &primes_cap, primes_len + 1);
            primes[primes_len++] = cand;

            /* Test the 30th‑power residue condition. */
            uint64_t g   = gcd_uint64(30ULL, cand - 1ULL);
            uint64_t exp = (cand - 1ULL) / g;
            uint64_t val = modpow_uint64(2ULL, exp, cand);
            if (val != 1ULL) {
                ensure_cap(&special, &special_cap, special_len + 1);
                special[special_len++] = cand;
            }
        }

        last_checked += 2; /* next odd candidate */
    }

    return (seq_t)special[(size_t)n - 1];
}
