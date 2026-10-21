/*
 * A085712  Semiprimes k such that gpf(k)^lpf(k)+1 is also semiprime, where
 *          gpf = A006530 and lpf = A020639.
 * https://oeis.org/A085712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A085712:  Semiprimes k such that gpf(k)^lpf(k)+1 is also semiprime.
    Only even semiprimes of the form k = 2·p (p odd prime) can satisfy the
    condition.  For such a k we need p²+1 to be a semiprime.  The sequence
    is thus  k_n = 2·p_n where p_n is the n‑th odd prime for which
    p²+1 has exactly two prime factors.  The implementation generates
    odd primes incrementally, tests the condition by trial division
    using a precomputed prime sieve, stores the found terms in a
    dynamic array and returns the requested element.  The function works
    for any seq_t/idx_t size (8,16,32,64,128 bits or arbitrary precision)
    as requested.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrime.h"  /* for bool isPrime(idx_t) */

/* ---------- constants ---------- */

/* Upper bound for primes used in the sieve.
   20 000 000 gives ≈1.3 million primes – plenty for the first 10 000
   terms of the sequence.  Adjusted to be safe. */
#define PRIME_SIEVE_LIMIT 20000000UL

/* ---------- global storage ---------- */

static uint32_t *prime_list = NULL;   /* array of primes up to limit */
static size_t prime_cnt = 0;          /* number of primes found */

static seq_t *terms = NULL;          /* stored terms of the sequence    */
static size_t terms_len = 0;          /* current number of terms stored  */
static size_t terms_cap = 0;          /* capacity of terms array         */

static size_t next_prime_index = 1;   /* index of next odd prime to test;
                                        prime_list[0] == 2, we start at 1 */

/* ---------- helper functions ---------- */

/* Simple Sieve of Eratosthenes stored as a byte array (1 = composite). */
static void build_prime_sieve(void)
{
    if (prime_list != NULL) return;   /* already built */

    size_t limit = PRIME_SIEVE_LIMIT;
    bool *is_comp = (bool *)malloc(limit + 1);
    if (!is_comp) exit(1);           /* allocation failure */

    for (size_t i = 0; i <= limit; ++i) is_comp[i] = false;
    is_comp[0] = is_comp[1] = true;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_comp[j] = true;
        }
    }

    /* Count primes */
    for (size_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) prime_cnt++;

    /* Allocate array */
    prime_list = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    if (!prime_list) exit(1);

    /* Store primes */
    size_t pos = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) prime_list[pos++] = (uint32_t)i;

    free(is_comp);
}

/* Test whether n is a semiprime (product of two primes, possibly equal).
   Assumes n > 1 and that prime_list contains all primes up to sqrt(n). */
static bool is_semiprime_uint64(uint64_t n)
{
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        uint64_t p64 = (uint64_t)p;
        if (p64 * p64 > n) break;          /* exceeded sqrt(n) */

        if (n % p64 == 0) {
            uint64_t q = n / p64;           /* remaining factor */
            return isPrime(q);              /* q must be prime */
        }
    }
    return false;    /* n is prime or has more than two prime factors */
}

/* Ensure that at least n terms of the sequence have been generated. */
static void generate_terms(size_t n)
{
    /* Build sieve and prime list on first use. */
    build_prime_sieve();

    /* Allocate terms array if needed. */
    if (terms == NULL) {
        terms_cap = 1024;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) exit(1);
    }

    /* Generate until we have n terms. */
    while (terms_len < n) {
        if (next_prime_index >= prime_cnt) {
            /* Should not happen with the chosen sieve limit. */
            break;
        }

        uint32_t p = prime_list[next_prime_index++];
        if (p == 2) continue;               /* skip the prime 2 */

        uint64_t val = (uint64_t)p * (uint64_t)p + 1ULL;   /* p²+1 */

        if (is_semiprime_uint64(val)) {
            seq_t term = (seq_t)((seq_t)2 * (seq_t)p);
            if (terms_len == terms_cap) {
                terms_cap *= 2;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (!terms) exit(1);
            }
            terms[terms_len++] = term;
        }
    }
}

/* ---------- API ---------- */

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n=0, just a safety net */
    generate_terms((size_t)n);
    return terms[(size_t)n - 1];
}
