/*
 * A063762  Composite integers k whose largest prime factor >= sqrt(k).
 * https://oeis.org/A063762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth composite integer k such that the largest prime
 *            factor of k is at least √k. Composed by generating numbers,
 *            testing the largest prime factor, and storing the qualifying
 *            composites in order. The sequence is generated lazily and cached
 *            so repeated calls are O(n). All computations use...
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   integer_sqrt_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/integer_sqrt_uint64.h"

/* forward declarations */
static uint32_t integer_sqrt_uint64(uint64_t);
static void ensure_primes_upto(uint32_t);
static void generate_next_prime(void);

/* Cache for the sequence */
static seq_t *terms = NULL;
static size_t  terms_len = 0;
static size_t  terms_cap = 0;

/* Cache for primes */
static uint32_t *primes = NULL;
static size_t   primes_len = 0;
static size_t   primes_cap = 0;
static uint32_t prime_candidate = 2;      /* next number to test for primality */

/* current candidate number to test */
static uint64_t current_k = 4;            /* start from 4 */

/* return the nth term of the sequence (1‑based) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but safe return */

    while (terms_len < (size_t)n) {
        uint64_t k = current_k;

        /* Ensure primes up to √k are available */
        uint32_t sqrtk = integer_sqrt_uint64(k);
        ensure_primes_upto(sqrtk);

        /* Factor k to find its largest prime divisor */
        uint64_t temp = k;
        uint64_t largest = 0;

        for (size_t i = 0; i < primes_len; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p * p > temp) break;
            if (temp % p == 0) {
                largest = p;
                do { temp /= p; } while (temp % p == 0);
            }
        }
        if (temp > 1) largest = temp;  /* remaining prime factor */

        /* Check for composite */
        if (largest < k) {  /* not a prime */
            /* Check if largest² ≥ k */
            unsigned __int128 largest_sq = (unsigned __int128)largest * largest;
            if (largest_sq >= (unsigned __int128)k) {
                /* Store term, expanding array if needed */
                if (terms_len == terms_cap) {
                    size_t newcap = terms_cap ? terms_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
                    terms = tmp;
                    terms_cap = newcap;
                }
                terms[terms_len++] = (seq_t)k;
            }
        }

        ++current_k;
    }

    return terms[(size_t)n - 1];
}

/* ---------- Helpers ---------- */

static void ensure_primes_upto(uint32_t limit)
{
    if (primes_len == 0 || primes[primes_len - 1] < limit) {
        while (primes_len == 0 || primes[primes_len - 1] < limit) {
            generate_next_prime();
        }
    }
}

static bool is_prime_candidate(uint32_t x)
{
    for (size_t i = 0; i < primes_len; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

static void generate_next_prime(void)
{
    while (true) {
        if (prime_candidate < 2) prime_candidate = 2;
        if (is_prime_candidate(prime_candidate)) {
            if (primes_len == primes_cap) {
                size_t newcap = primes_cap ? primes_cap * 2 : 16;
                uint32_t *tmp = (uint32_t *)realloc(primes, newcap * sizeof(uint32_t));
                primes = tmp;
                primes_cap = newcap;
            }
            primes[primes_len++] = prime_candidate;
            ++prime_candidate;
            break;
        }
        ++prime_candidate;
    }
}
