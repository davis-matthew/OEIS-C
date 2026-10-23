/*
 * A141123  Primes of the form -x^2+2*x*y+2*y^2 (as well as of the form
 *          3*x^2+6*x*y+2*y^2).
 * https://oeis.org/A141123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1566, u32 n <= 1567, u64 n <= 6514,
 *            u128 n <= 6514, big any n
 * Helpers:   append_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) is 2, then 3, then the primes that are congruent to 11 (mod 12). */
/* The implementation builds the sequence on demand, caching results. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"    /* bool is_prime_big(seq_t n); */
#include "../utils/append_candidate.h" /* void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val); */

static seq_t *cache = NULL;   /* cached sequence terms */
static size_t cache_len = 0;   /* number of cached terms */
static size_t cache_cap = 0;   /* capacity of the cache array */
static idx_t next_candidate = 2;   /* next integer to test */

/* Return the nth term (1‑based) of A141123. */
seq_t a(idx_t n)
{
    /* Fast path: return cached value if already computed. */
    if (cache_len >= (size_t)n)
        return cache[n - 1];

    /* Extend the sequence until we have at least n terms. */
    while (cache_len < (size_t)n)
    {
        /* Find the next prime that satisfies the required form. */
        while (true)
        {
            if (is_prime_big((seq_t)next_candidate) &&
                (next_candidate == 2 || next_candidate == 3 ||
                 (next_candidate % 12) == 11))
            {
                /* Store it in the cache. */
                append_candidate(&cache, &cache_len, &cache_cap,
                                 (seq_t)next_candidate);
                /* Move past this prime for the next search. */
                ++next_candidate;
                break;
            }
            ++next_candidate;
        }
    }

    return cache[n - 1];
}
