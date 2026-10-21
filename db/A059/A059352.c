/*
 * A059352  Primes p such that x^40 = 2 has no solution mod p.
 * https://oeis.org/A059352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A059352
   Primes p such that the congruence x^40 ≡ 2 (mod p) has no solution.
   For an odd prime p, let d = gcd(40, p-1).  Then the congruence is solvable
   iff 2^((p-1)/d) ≡ 1 (mod p).  A prime appears in the sequence when
   this congruence is not satisfied. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/mod_pow.h"

static inline uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* --- static state --- */
    static seq_t *results      = NULL;
    static size_t results_len  = 0;
    static size_t results_cap  = 0;

    static uint64_t *prime_list = NULL;
    static size_t primes_len    = 0;
    static size_t primes_cap    = 0;
    static uint64_t next_candidate = 2;

    /* ---- generate terms until we have at least n ---- */
    while (results_len < n) {
        /* find next prime > 2 */
        uint64_t p = 0;
        while (true) {
            /* choose candidate */
            uint64_t cand = next_candidate;
            if (cand <= 2) {
                cand = 2;
                next_candidate = 3;
            } else {
                if ((cand & 1) == 0) cand++;          /* make odd */
                next_candidate = cand + 1;
            }

            /* primality test by trial division using known primes */
            bool is_prime = true;
            for (size_t i = 0; i < primes_len; ++i) {
                uint64_t q = prime_list[i];
                if ((uint64_t)q * q > cand) break;
                if (cand % q == 0) {
                    is_prime = false;
                    break;
                }
            }

            if (is_prime) {
                p = cand;
                break;
            }
        }

        /* store the prime in the list */
        if (primes_len == primes_cap) {
            size_t newcap = primes_cap ? primes_cap * 2 : 16;
            uint64_t *tmp = (uint64_t *)realloc(prime_list, newcap * sizeof(uint64_t));
            if (!tmp) abort();
            prime_list = tmp;
            primes_cap = newcap;
        }
        prime_list[primes_len++] = p;

        /* skip p = 2 (not in the sequence) */
        if (p <= 2) continue;

        /* test the condition */
        uint64_t d   = gcd_u64(40ULL, p - 1ULL);
        uint64_t exp = (p - 1ULL) / d;
        seq_t modpow = mod_pow((seq_t)2, exp, (seq_t)p);
        if (modpow != (seq_t)1) {
            /* append to results */
            if (results_len == results_cap) {
                size_t newcap = results_cap ? results_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(results, newcap * sizeof(seq_t));
                if (!tmp) abort();
                results = tmp;
                results_cap = newcap;
            }
            results[results_len++] = (seq_t)p;
        }
    }

    return results[n - 1];
}
