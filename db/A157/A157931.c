/*
 * A157931  Numbers that are both the sum and the product of two primes.
 * https://oeis.org/A157931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 5643, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A157931 – Numbers that are both the sum and the product of two primes.
 *
 * A(n) is the nth smallest integer that can be written as a product of two
 * primes and also as a sum of two primes.  The sequence starts  
 * 4, 6, 9, 10, 14, 15, 21, 22, 25, 26, 33, 34, 38, 39, ...
 *
 * The algorithm generates the sequence on demand, extending a prime sieve
 * and a semiprime test as needed.  The sequence is stored in a static
 * cache so that arbitrary order of calls is supported.
 * The implementation uses only integer arithmetic and standard C
 * facilities; it works for any integer type that the build supplies
 * for `seq_t` and `idx_t`.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* --------------------------------------------------------------- */
/*  Prime table and smallest‑prime‑factor table (spf).  */
static idx_t          sieve_limit   = 0;   /* current allocated limit */
static bool           *is_prime     = NULL;
static uint32_t       *spf          = NULL;

static size_t         prime_cap     = 0;
static size_t         prime_count   = 0;
static uint32_t       *primes       = NULL;

/* Re‑build primes and spf tables up to 'limit' (inclusive). */
static void
ensure_limit(idx_t limit)
{
    if (limit <= sieve_limit) return;          /* already large enough */

    free(is_prime);
    free(spf);
    free(primes);

    sieve_limit = limit;
    is_prime = (bool *)malloc((size_t)(sieve_limit + 1) * sizeof(bool));
    spf     = (uint32_t *)malloc((size_t)(sieve_limit + 1) * sizeof(uint32_t));

    for (idx_t i = 0; i <= sieve_limit; ++i) {
        is_prime[i] = true;  /* assume prime */
        spf[i] = 0;          /* 0 will mean 'no smallest prime factor yet' */
    }
    if (sieve_limit >= 0) {
        is_prime[0] = false;
        if (sieve_limit >= 1) is_prime[1] = false;
    }

    prime_cap   = 0;
    prime_count = 0;
    primes = NULL;

    for (idx_t i = 2; i <= sieve_limit; ++i) {
        if (is_prime[i]) {                 /* i is prime */
            /* store prime */
            if (prime_count == prime_cap) {
                prime_cap = prime_cap == 0 ? 256 : prime_cap * 2;
                primes = (uint32_t *)realloc(primes, prime_cap * sizeof(uint32_t));
            }
            primes[prime_count++] = (uint32_t)i;
            spf[i] = (uint32_t)i;           /* smallest prime factor of a prime is itself */

            if ((uint64_t)i * (uint64_t)i <= sieve_limit) {
                for (uint64_t j = (uint64_t)i * (uint64_t)i;
                     j <= sieve_limit; j += i) {
                    if (is_prime[(size_t)j]) is_prime[(size_t)j] = false;
                    if (spf[(size_t)j] == 0) spf[(size_t)j] = (uint32_t)i;
                }
            }
        }
    }
}

/* --------------------------------------------------------------- */
/*  Sequence cache.   */
static size_t            term_cap     = 0;
static size_t            term_count   = 0;   /* numbers found so far */
static seq_t            *terms        = NULL;
static idx_t             last_checked = 3;   /* last candidate examined */

/* Append new term to cache. */
static void
add_term(seq_t val)
{
    if (term_count == term_cap) {
        term_cap = term_cap == 0 ? 256 : term_cap * 2;
        terms = (seq_t *)realloc(terms, term_cap * sizeof(seq_t));
    }
    terms[term_count++] = val;
}

/* --------------------------------------------------------------- */
/*  Main function requested by the build.   */
seq_t
a(idx_t n)
{
    /* Ensure we have at least 'n' terms. */
    while (term_count < n) {
        idx_t cand = last_checked + 1;

        /* Extend sieve if necessary (extend generously). */
        if (cand > sieve_limit) {
            idx_t new_limit = cand * 2;
            if (new_limit < cand) new_limit = UINT64_MAX;  /* handle overflow */
            ensure_limit(new_limit);
        }

        /* Test whether 'cand' is a semiprime. */
        bool is_semiprime = false;
        if (cand >= 4) {
            uint32_t p = spf[(size_t)cand];
            if (p && p != (uint32_t)cand) {
                idx_t q = cand / p;
                if (q >= 2 && is_prime[(size_t)q])   /* the other factor is prime */
                    is_semiprime = true;
            }
        }

        /* Test whether 'cand' is a sum of two primes.  Even numbers
           are assumed to satisfy Goldbach; odd numbers need cand-2 prime. */
        bool is_sum = (cand & 1ULL) == 0 ? true : is_prime[(size_t)(cand - 2)];

        if (is_semiprime && is_sum) {
            add_term((seq_t)cand);          /* store the term */
        }

        last_checked = cand;
    }

    return terms[n - 1];   /* one‑based index */
}
