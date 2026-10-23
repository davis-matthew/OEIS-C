/*
 * A139513  Primes congruent to {1, 3, 7, 9} mod 20.
 * https://oeis.org/A139513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime p with p ≡ 1,3,7, or 9 (mod 20). The
 *            function builds the list incrementally and memoises it. It may
 *            be called with indices in any order
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"

/* Cached sequence: prime_cache[k] == a(k+1) */
static seq_t *prime_cache = NULL;
static idx_t  cache_len   = 0;
static idx_t  cache_cap   = 0;

/* Next odd integer to test for primality */
static idx_t next_candidate = 3;

/* Extend the cache until it contains at least n terms */
static void extend_cache_until(idx_t n)
{
    while (cache_len < n) {
        /* Test current candidate for primality */
        if (is_prime_small((seq_t)next_candidate)) {
            /* Accept only primes congruent to 1,3,7,9 modulo 20 */
            idx_t mod20 = next_candidate % 20;
            if (mod20 == 1 || mod20 == 3 || mod20 == 7 || mod20 == 9) {
                /* Grow array if needed */
                if (cache_len == cache_cap) {
                    idx_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(prime_cache,
                                                  (size_t)new_cap * sizeof(seq_t));
                    if (!tmp) abort();          /* memory exhaustion */
                    prime_cache = tmp;
                    cache_cap   = new_cap;
                }
                prime_cache[cache_len++] = (seq_t)next_candidate;
            }
        }
        /* Advance to the next odd integer */
        next_candidate += 2;
    }
}

/* Main entry point for the sequence */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* defined only for n ≥ 1 */
    if (cache_len < n) extend_cache_until(n);
    return prime_cache[n - 1];
}
