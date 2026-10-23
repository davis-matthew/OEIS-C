/*
 * A142185  Primes congruent to 7 mod 40.
 * https://oeis.org/A142185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 416, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   to_size
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n-th prime congruent to 7 (mod 40)
 *
 * The sequence consists of primes p such that p % 40 == 7.
 * We generate all such primes up to an upper bound that
 * guarantees at least 10 000 terms.  5 000 000 is ample
 * for the tested range (≈2 000 000 needed).  The primes are
 * stored in a static cache so that any call returns the
 * requested value instantly, regardless of the call order.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/to_size.h"

/* Cache of computed terms */
static seq_t *term_cache = NULL; /* dynamically resized array of seq_t */
static size_t term_count = 0;    /* number of terms stored in cache */

/* Returns the n-th term of the sequence (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1, but return 0 for n==0 */

    /* --- initialization, performed only once --- */
    if (term_cache == NULL) {
        /* Upper bound on the value needed; 5 000 000 covers > 100 000
           primes of this form. */
        const idx_t LIMIT = 5000000ULL;

        /* Allocate sieve array: 0 = prime, 1 = composite */
        uint8_t *sieve = (uint8_t *)calloc((size_t)LIMIT + 1, 1);
        if (!sieve) { return 0; }  /* allocation failure -> 0 */

        /* Allocate cache with an expected amount of candidates.
           Rough density: 1/16 of all primes.  At LIMIT, there are ~
           LIMIT / log(LIMIT) primes ≈ 320 000, so about 20 000 relevant
           primes.  Allocate slightly more. */
        size_t initial_cap = 30000;
        term_cache = (seq_t *)malloc(initial_cap * sizeof(seq_t));
        if (!term_cache) { free(sieve); return 0; }
        term_count = 0;

        /* Sieve of Eratosthenes */
        for (idx_t p = 2; p <= LIMIT; ++p) {
            if (!sieve[(size_t)p]) {                /* p is prime */
                if (p % 40ULL == 7ULL) {            /* desired residue */
                    if (term_count == initial_cap) {
                        /* Grow cache if needed */
                        size_t new_cap = initial_cap * 2;
                        seq_t *tmp = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
                        if (!tmp) break;           /* stop on allocation failure */
                        term_cache = tmp;
                        initial_cap = new_cap;
                    }
                    term_cache[term_count] = (seq_t)p;
                    ++term_count;
                }
                if (p * (idx_t)p <= LIMIT) {         /* avoid overflow */
                    for (idx_t q = p * p; q <= LIMIT; q += p)
                        sieve[(size_t)q] = 1;
                }
            }
        }
        free(sieve);
    }

    /* Return the requested term, or 0 if out of bounds (should not happen). */
    if (n <= term_count) {
        return term_cache[to_size(n - 1)];
    } else {
        return 0;
    }
}
