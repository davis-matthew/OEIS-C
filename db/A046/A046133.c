/*
 * A046133  Primes p such that p + 12 is also prime.
 * https://oeis.org/A046133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes A046133: the sequence of primes p such that
   p+12 is also prime.  It grows an internal cache of the found terms
   and extends it as needed.  No hard‑coded terms are used.  */
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    /* static cache variables */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static idx_t current = 2;   /* start before the smallest prime */

    /* Extend the cache until we have at least n terms */
    while (len < n) {
        current = next_prime(current);          /* next prime > current */
        if (is_prime(current + 12)) {
            append_candidate(&cache, &len, &cap, (seq_t)current);
        }
    }

    return cache[n - 1];
}
