/*
 * A227030  Numbers that are neither the difference nor the product of two
 *          primes.
 * https://oeis.org/A227030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numbers that are neither the difference nor the product
 *            of two primes
 * Range:     u8 n <= 44, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   build_sieve_and_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/build_sieve_and_primes.h"

static seq_t *terms = NULL;
static size_t   terms_len = 0;
static size_t   terms_cap = 0;
static idx_t    scanned_up_to = 0;

static void ensure_terms_cap(size_t min_cap) {
    if (min_cap > terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        if (new_cap < min_cap) new_cap = min_cap;
        seq_t *new_arr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!new_arr) { /* OOM: abort */ exit(1); }
        terms = new_arr;
        terms_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined, but return 0 */
    while (terms_len < (size_t)n) {
        /* Increase the search limit */
        idx_t new_limit = (scanned_up_to == 0) ? 10 : scanned_up_to * 2;
        if (new_limit < scanned_up_to + 10) new_limit = scanned_up_to + 10 ;
        /* For difference detection use primes up to max(new_limit*2, new_limit+2) */
        idx_t prime_limit = new_limit * 2;
        if (prime_limit < new_limit + 2) prime_limit = new_limit + 2;

        /* Build sieve and primes up to prime_limit */
        bool *is_prime = NULL;
        uint64_t *primes = NULL;
        size_t prime_count = 0;
        build_sieve_and_primes(prime_limit, &is_prime, &primes, &prime_count);

        /* Mark excluded numbers up to new_limit */
        bool *excluded = (bool *)calloc((size_t)(new_limit + 1), sizeof(bool));
        if (!excluded) exit(1);

        /* Product of two primes */
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t pi = primes[i];
            /* Stop early if pi > new_limit */
            if (pi > new_limit) break;
            for (size_t j = 0; j <= i; ++j) {
                uint64_t pj = primes[j];
                unsigned long long prod = pi * pj;
                if (prod > (unsigned long long)new_limit) break;
                excluded[(idx_t)prod] = true;
            }
        }

        /* Difference of two primes (p > q) */
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t pi = primes[i];
            for (size_t j = 0; j < i; ++j) {
                idx_t diff = (idx_t)(pi - primes[j]);  /* pi > primes[j] */
                if (diff > new_limit) continue;
                excluded[diff] = true;
            }
        }

        /* Scan new candidate range and store non-excluded terms */
        for (idx_t cand = scanned_up_to + 1; cand <= new_limit; ++cand) {
            if (!excluded[cand]) {
                ensure_terms_cap(terms_len + 1);
                terms[terms_len++] = (seq_t)cand;
            }
        }

        /* Clean up */
        free(is_prime);
        free(primes);
        free(excluded);
        scanned_up_to = new_limit;
    }
    return terms[n - 1];
}
