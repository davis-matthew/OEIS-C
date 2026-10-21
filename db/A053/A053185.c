/*
 * A053185  Primes of the form p^2 + p - 1 when p is prime.
 * https://oeis.org/A053185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = p^2 + p - 1, where p is prime and a(n) itself is prime
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 1059, u64 n <= 1200,
 *            u128 n <= 1200, big any n
 * Helpers:   is_prime64, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the n-th term of OEIS sequence A053185. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_prime64.h"
#include "../utils/sieve_primes.h"

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined, but safe fallback */

    /* Static state to memoize computed terms */
    static idx_t current_sieve_limit = 4;
    static idx_t *prime_cache = NULL;
    static size_t prime_count = 0;
    static seq_t *term_cache = NULL;
    static size_t term_count = 0;

    /* Ensure we have at least n terms */
    while (term_count < (size_t)n) {
        /* Extend sieve limit (double each time) */
        current_sieve_limit *= 2;

        /* Generate primes up to new limit */
        if (prime_cache) free(prime_cache);
        sieve_primes(current_sieve_limit, &prime_cache, &prime_count);

        /* Recompute all terms from scratch up to new limit */
        size_t new_term_count = 0;
        seq_t *new_terms = NULL;

        /* Reserve enough space: worst case every prime yields term */
        new_terms = (seq_t*)realloc(new_terms, prime_count * sizeof(seq_t));
        if (!new_terms) break; /* allocation failure, abort */

        for (size_t i = 0; i < prime_count; ++i) {
            idx_t p = prime_cache[i];
            unsigned __int128 v = (unsigned __int128)p * p + p - 1;
            uint64_t v64 = (uint64_t)v; /* v fits in 64 bits for our limits */
            if (is_prime64(v64)) {
                new_terms[new_term_count++] = (seq_t)v;
            }
        }

        /* Replace old caches */
        if (term_cache) free(term_cache);
        term_cache = new_terms;
        term_count = new_term_count;
    }

    /* Return the requested term (1-based index) */
    return term_cache[n - 1];
}
