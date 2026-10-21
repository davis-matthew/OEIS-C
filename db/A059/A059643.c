/*
 * A059643  Primes p such that x^57 = 2 has no solution mod p.
 * https://oeis.org/A059643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th prime p such that 2 is not a 57‑th power residue
 *            modulo p, i.e. the congruence x^57 ≡ 2 (mod p) has no solution
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_u64, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"
#include "../utils/gcd_u64.h"

/* helper: greatest common divisor for uint64_t */

/* static state for memoisation */
static uint64_t *prime_list = NULL;   /* all primes found so far          */
static size_t   prime_len  = 0;
static size_t   prime_cap  = 0;

static seq_t *found_primes = NULL;   /* primes that satisfy the property */
static size_t   found_len   = 0;
static size_t   found_cap   = 0;

static uint64_t last_candidate = 2;
static bool initialized = false;

/* add a prime to prime_list */
static void add_prime(uint64_t p)
{
    if (prime_len == prime_cap) {
        prime_cap = prime_cap ? prime_cap * 2 : 8;
        uint64_t *tmp = (uint64_t*)realloc(prime_list,
                                            prime_cap * sizeof(uint64_t));
        if (!tmp) abort();
        prime_list = tmp;
    }
    prime_list[prime_len++] = p;
}

/* add a found prime to found_primes */
static void add_found_prime(seq_t p)
{
    if (found_len == found_cap) {
        found_cap = found_cap ? found_cap * 2 : 8;
        seq_t *tmp = (seq_t*)realloc(found_primes,
                                    found_cap * sizeof(seq_t));
        if (!tmp) abort();
        found_primes = tmp;
    }
    found_primes[found_len++] = p;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        prime_cap   = 8;
        prime_list  = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_list) abort();
        add_prime(2);

        found_cap   = 8;
        found_primes = (seq_t*)malloc(found_cap * sizeof(seq_t));
        if (!found_primes) abort();

        last_candidate = 2;
        initialized = true;
    }

    while (found_len < n) {
        /* generate next candidate prime */
        do {
            last_candidate++;

            /* skip even numbers > 2 */
            if (last_candidate > 2 && (last_candidate & 1ULL) == 0) continue;

            /* simple primality test using known primes */
            bool is_prime = true;
            for (size_t i = 0; i < prime_len &&
                                 prime_list[i] * prime_list[i] <= last_candidate;
                 ++i) {
                if (last_candidate % prime_list[i] == 0) {
                    is_prime = false;
                    break;
                }
            }

            if (is_prime) {
                add_prime(last_candidate);

                /* Skip p = 2 – not in the required list */
                if (last_candidate == 2) continue;

                uint64_t g   = gcd_u64(57ULL, last_candidate - 1);
                uint64_t exp = (last_candidate - 1) / g;
                seq_t res = mod_pow((seq_t)2, exp, (seq_t)last_candidate);

                if (res != (seq_t)1) {
                    add_found_prime((seq_t)last_candidate);
                }
            }
        } while (found_len < n);
    }

    return found_primes[n - 1];
}
