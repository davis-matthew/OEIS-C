/*
 * A142096  Primes congruent to 31 mod 35.
 * https://oeis.org/A142096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 236, u32 n <= 249, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of primes congruent to 31 (mod 35) */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* declares bool is_prime_big(seq_t n) */

static seq_t *cache = NULL;          /* array of computed terms        */
static idx_t  cache_size = 0;        /* number of terms currently stored */
static idx_t  cache_cap   = 0;        /* allocated capacity (in seq_t items) */
static seq_t  last_prime = 0;        /* last prime that satisfied the condition */

/* Ensure that the cache can hold at least 'needed' elements */
static void ensure_cache_capacity(idx_t needed)
{
    if (needed <= cache_cap) return;

    idx_t new_cap = cache_cap > 0 ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th prime ≡ 31 (mod 35).  n ≥ 1 is assumed. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out-of-range request (value unspecified) */

    /* Return cached value if already available */
    if (n <= cache_size) return cache[n - 1];

    /* Generate additional terms up to n */
    while (cache_size < n)
    {
        seq_t candidate;
        if (cache_size == 0) {
            /* First term */
            candidate = (seq_t)31;
        } else {
            /* Next candidate is last_prime + 35 */
            candidate = last_prime + (seq_t)35;
        }

        /* Find the next prime of the form 31 + 35*k */
        while (!is_prime_big(candidate)) {
            candidate += (seq_t)35;
        }

        /* Store the found prime */
        ensure_cache_capacity(cache_size + 1);
        cache[cache_size++] = candidate;
        last_prime = candidate;
    }

    return cache[n - 1];
}
