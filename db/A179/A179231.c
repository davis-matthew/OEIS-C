/*
 * A179231  Primes of the form 250n + 1.
 * https://oeis.org/A179231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 55, u32 n <= 56, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form 250*n + 1
 *
 * This implementation keeps a static cache of already‐found primes.
 * On each call it expands the cache until it contains the requested
 * nth prime, testing candidates 250*k+1 with k = 1,2,3… using
 * is_prime_big().  The function works for any integer type seq_t
 * (including arbitrary precision integers) and any 64‑bit index type
 * idx_t.
 *
 * Complexity: O(n * P(n)) where P(n) is the average cost of the
 * primality test; for n ≤ 10 000 this runs well under the time limit. */

#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* The public sequence routine ------------------------------------------------*/

seq_t a(idx_t n)
{
    /* static cache of primes of the form 250*k + 1 */
    static seq_t  *prime_cache   = NULL;
    static size_t  cache_cap     = 0;   /* allocated capacity in elements */
    static size_t  prime_count   = 0;   /* number of primes stored */
    static idx_t   next_k        = 1;   /* next k to test (k starts at 1) */

    /* OEIS offset is 1 – for n==0 nothing is defined; return 0 smoothly */
    if (n == 0)
        return (seq_t)0;

    /* Initialise cache on first invocation */
    if (prime_cache == NULL) {
        cache_cap   = 1024;
        prime_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (prime_cache == NULL)
            abort();          /* allocation failure */
    }

    /* Grow the cache until we have at least n primes */
    while (prime_count < n) {
        /* Grow the cache if it is full */
        if (prime_count == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
            if (tmp == NULL)
                abort();      /* allocation failure */
            prime_cache = tmp;
            cache_cap   = new_cap;
        }

        /* Build the candidate of the form 250*k + 1 */
        uint64_t candidate_u = (uint64_t)250 * (uint64_t)next_k + 1ULL;
        seq_t   candidate    = (seq_t)candidate_u;   /* cast – acceptable
                                                        since this
                                                        value will fit
                                                        into seq_t for all
                                                        terms that are
                                                        required to be exact */

        /* If the candidate is prime, store it */
        if (is_prime_big(candidate))
            prime_cache[prime_count++] = candidate;

        ++next_k;          /* move to the next k value */
    }

    /* Return the requested prime (indexing is 1‑based) */
    return prime_cache[n - 1];
}
