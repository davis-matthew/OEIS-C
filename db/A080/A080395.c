/*
 * A080395  Even numbers k such that the central binomial coefficient
 *          A000984(k, k/2) is divisible by k^2.
 * https://oeis.org/A080395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 33, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   legendre_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A080395:
   a(n) = the nth even integer k such that the central binomial coefficient C(k,k/2)
   is divisible by k².  (Equivalently, for every prime p|k,  p^ (2 v_p(k)) | C(k,k/2).) */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/legendre_exp.h"

/* ------------------------------------------------------------------ */
/* Prime precomputation ------------------------------------------------ */
static bool primes_initialized = false;
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;
static const uint32_t PRIME_SIEVE_LIMIT = 200000;   /* > sqrt(max expected k) */

static void init_primes(void)
{
    if (primes_initialized) return;
    bool *is_comp = (bool*)calloc(PRIME_SIEVE_LIMIT + 1, sizeof(bool));
    for (uint32_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!is_comp[i]) {
            if (prime_count == 0) { /* allocate lazily */
                prime_list = NULL;
            }
            prime_list = (uint32_t*)realloc(prime_list, (prime_count + 1) * sizeof(uint32_t));
            prime_list[prime_count++] = i;
            if ((uint64_t)i * i <= PRIME_SIEVE_LIMIT) {
                for (uint64_t j = (uint64_t)i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                    is_comp[(size_t)j] = true;
            }
        }
    }
    free(is_comp);
    primes_initialized = true;
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Check whether a given even k satisfies the divisibility condition  */
static inline bool good_even_k(idx_t k)
{
    idx_t half = k >> 1;          /* k/2 */
    idx_t tmp = k;                /* remaining factor */
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * p > tmp) break;
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            int vC = legendre_exp(k, p) - 2 * legendre_exp(half, p);
            if (vC < 2 * e) return false;
        }
    }
    if (tmp > 1) {                /* remaining prime factor */
        uint32_t p = (uint32_t)tmp;
        int e = 1;
        int vC = legendre_exp(k, p) - 2 * legendre_exp(half, p);
        if (vC < 2 * e) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Dynamic list of found terms --------------------------------------- */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;
static idx_t next_k = 2;  /* next even integer to test */

/* Extend the list until it contains at least 'need' terms. */
static void make_terms_upto(size_t need)
{
    if (!primes_initialized) init_primes();

    /* ensure capacity for terms */
    if (terms_cap == 0) {
        terms_cap = 16;
        terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
    }

    while (terms_len < need) {
        /* look for next good even k */
        while (true) {
            if (good_even_k(next_k)) {
                /* store value */
                if (terms_len == terms_cap) {
                    terms_cap *= 2;
                    terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
                }
                terms[terms_len++] = (seq_t)next_k;
                break;
            }
            next_k += 2;   /* only even numbers */
        }
        /* make sure we test sufficiently large k next time */
        next_k += 2;
    }
}

/* ------------------------------------------------------------------ */
/* Public interface -------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* convention for out of bounds */
    make_terms_upto((size_t)n);
    return terms[n - 1];
}
