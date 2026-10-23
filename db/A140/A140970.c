/*
 * A140970  Primes of the form (k-1)! + k^(k-1).
 * https://oeis.org/A140970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, more
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   big_pow, is_prime_big, seq_alloc, seq_one, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of sequence A140970: primes of the form (k‑1)! + k^(k‑1) */

#include <stdbool.h>
#include "../utils/big_pow.h"     /* seq_t big_pow(seq_t base, seq_t exp) */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t n) */
#include "../utils/seq_alloc.h"   /* seq_t* seq_alloc(size_t n) */
#include "../utils/seq_realloc.h" /* seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) */
#include "../utils/seq_one.h"     /* seq_t seq_one(void) */

seq_t a(idx_t n)
{
    /* Static state: cache of primes found, current factorial, and last k examined. */
    static seq_t *prime_cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_k = 0;          /* last k processed */
    static seq_t fact = 0;            /* fact holds (k-1)! for the current k */

    /* Initialise on first call. */
    if (prime_cache == NULL) {
        cache_cap = 64;                      /* initial cache capacity */
        prime_cache = seq_alloc(cache_cap);
        cache_len   = 0;
        last_k      = 0;
        fact        = seq_one();            /* fact = 0! = 1 for k = 1 */
    }

    /* Return cached result if already computed. */
    if (n <= cache_len) {
        return prime_cache[n - 1];
    }

    /* Generate primes until we have at least n of them. */
    while (cache_len < n) {
        ++last_k;                             /* Next k to test (starting from 1) */

        /* Compute k^(k-1) using big_pow. */
        seq_t pow_val = big_pow((seq_t)last_k, (seq_t)(last_k - 1));

        /* Candidate value: (k-1)! + k^(k-1). */
        seq_t candidate = fact + pow_val;

        /* Store if it is prime. */
        if (is_prime_big(candidate)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap << 1;
                prime_cache = seq_realloc(prime_cache, cache_cap, new_cap);
                cache_cap   = new_cap;
            }
            prime_cache[cache_len++] = candidate;
        }

        /* Update factorial for next iteration: fact = k! */
        fact *= (seq_t)last_k;
    }

    return prime_cache[n - 1];
}
