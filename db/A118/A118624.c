/*
 * A118624  Primes of the form prime(n+1)*prime(n+3) - prime(n)*prime(n+2) -
 *          1, ordered by n.
 * https://oeis.org/A118624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 84, u32 n <= 43, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A118624: Primes of the form prime(n+1)*prime(n+3) – prime(n)*prime(n+2) – 1,
   ordered by n.
   a(1) = 59, a(2) = 359, a(3) = 197, … */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

/* Static cache of already discovered terms. */
static seq_t *cache     = NULL; /* array of terms */
static size_t cache_cap = 0;    /* allocated capacity */
static size_t cache_sz  = 0;    /* number of terms actually stored */

/* Ensure the cache can hold at least ‘needed’ elements.  Proceeds
   by doubling the capacity until it is big enough. */
static void ensure_cache_cap(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < needed) new_cap *= 2;
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }
}

/* Main function: return the n‑th term of the sequence.
   Calls to this function can be made in arbitrary order. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 – return 0 */

    static idx_t last_checked_n = 0;     /* last formula index examined */

    /* Compute terms until we have at least ‘n’ primes. */
    while (cache_sz < n) {
        ++last_checked_n;                /* formula index for this iteration */

        /* Retrieve the four primes required by the formula. */
        seq_t p0 = (seq_t)get_prime(last_checked_n);     /* prime(n)   */
        seq_t p1 = (seq_t)get_prime(last_checked_n + 1); /* prime(n+1) */
        seq_t p2 = (seq_t)get_prime(last_checked_n + 2); /* prime(n+2) */
        seq_t p3 = (seq_t)get_prime(last_checked_n + 3); /* prime(n+3) */

        /* Candidate value: p1*p3 – p0*p2 – 1. */
        seq_t cand = p1 * p3 - p0 * p2 - (seq_t)1;

        /* Store it if it is prime. */
        if (is_prime_big(cand)) {
            ensure_cache_cap(cache_sz + 1);
            cache[cache_sz++] = cand;
        }
    }

    return cache[n - 1];
}
