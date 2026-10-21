/*
 * A070542  Numbers n such that binomial(2n,n) does not have two prime factors
 *          that add to 2n.
 * https://oeis.org/A070542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   exp_in_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A070542   Numbers n such that binomial(2n,n) does not
 *  have two prime factors that add up to 2n.
 *  This implementation searches integers n and keeps a cache
 *  of those that satisfy the condition.  Prime sieving is
 *  performed incrementally up to 2n.  All computations use
 *  only integer arithmetic and respect the types seq_t and
 *  idx_t supplied by the build. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/exp_in_binom.h"

/* Prime sieve state */
static uint8_t *is_prime = NULL;   /* prime flag array */
static size_t is_prime_size = 0;   /* allocated size of is_prime */
static uint32_t *primes = NULL;    /* list of primes found */
static size_t primes_size = 0;     /* number of primes stored */
static idx_t current_limit = 1;    /* highest value covered */

/* Incrementally extend the prime table to cover all numbers
   up to new_limit (inclusive).  It only recomputes the new
   range and uses the old primes to cross off composites. */
static void extend_primes(idx_t new_limit)
{
    if (new_limit <= current_limit)
        return;                 /* nothing to do */

    size_t old_size = is_prime_size;
    size_t new_size = (size_t)new_limit + 1;

    /* Allocate or grow the flag array */
    if (old_size == 0) {
        is_prime = (uint8_t *)calloc(new_size, 1);
        for (size_t i = 0; i < new_size; ++i)
            is_prime[i] = 1;
    } else {
        is_prime = (uint8_t *)realloc(is_prime, new_size);
        for (size_t i = old_size; i < new_size; ++i)
            is_prime[i] = 1;
    }
    if (new_size > 0)  is_prime[0] = 0;
    if (new_size > 1)  is_prime[1] = 0;

    /* Cross off multiples of known primes in the new range */
    for (size_t pi = 0; pi < primes_size; ++pi) {
        uint32_t p = primes[pi];
        uint64_t start = (uint64_t)(old_size + p - 1) / p * p;
        if (start < (uint64_t)p * p)
            start = (uint64_t)p * p;
        for (uint64_t m = start; m <= (uint64_t)new_limit; m += p)
            is_prime[(size_t)m] = 0;
    }

    /* Scan the new zone to find primes, extend the prime list,
       and cross off multiples of these new primes. */
    for (size_t i = old_size; i <= new_limit; ++i) {
        if (is_prime[(size_t)i]) {
            /* add to prime list */
            primes = (uint32_t *)realloc(primes,
                    (primes_size + 1) * sizeof(uint32_t));
            primes[primes_size++] = (uint32_t)i;

            /* cross off multiples of this new prime if needed */
            if ((uint64_t)i * i <= (uint64_t)new_limit) {
                for (uint64_t m = (uint64_t)i * i; m <= (uint64_t)new_limit; m += i)
                    is_prime[(size_t)m] = 0;
            }
        }
    }

    is_prime_size = new_size;
    current_limit  = new_limit;
}

/* Cache of sequence terms */
static seq_t *terms = NULL;
static size_t terms_size = 0;   /* number of terms stored */
static size_t terms_cap  = 0;   /* capacity of the terms array */
static idx_t  last_candidate = 0;   /* last integer that was examined */

seq_t a(idx_t n)
{
    /* Ensure we have produced at least n terms */
    while (terms_size < n) {
        ++last_candidate;              /* examine the next integer */

        /* Make sure we have prime information up to 2*last_candidate */
        extend_primes(2 * last_candidate);

        /* Gather the distinct prime factors of binomial(2n,n) */
        uint32_t fp[256];    /* factor primes */
        int     fe[256];     /* corresponding exponents */
        size_t  fp_cnt = 0;

        for (size_t pi = 0; pi < primes_size &&
                         primes[pi] <= 2 * last_candidate; ++pi) {
            uint32_t p = primes[pi];
            int e = exp_in_binom(last_candidate, p);
            if (e > 0) {
                fp[fp_cnt]   = p;
                fe[fp_cnt++] = e;
            }
        }

        /* Check whether any two (possibly equal) factors sum to 2n */
        bool ok = true;
        for (size_t i = 0; i < fp_cnt && ok; ++i) {
            for (size_t j = i; j < fp_cnt && ok; ++j) {
                uint64_t sum = (uint64_t)fp[i] + (uint64_t)fp[j];
                if (sum == 2ULL * (uint64_t)last_candidate) {
                    if (i == j) {             /* same prime, need exponent >= 2 */
                        if (fe[i] < 2) continue;
                    }
                    ok = false;
                }
            }
        }

        if (ok) {       /* store the term */
            if (terms_size == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 4;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_size++] = (seq_t)last_candidate;
        }
    }

    return terms[n - 1];
}
