/*
 * A142388  Primes congruent to 37 mod 47.
 * https://oeis.org/A142388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 109, u32 n <= 110, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime congruent to 37 (mod 47).
   The sequence is defined by a(1)=37 and each subsequent term is
   the next prime ≡ 37 (mod 47).  No closed‑form expression is known. */

#include <stddef.h>
#include "../utils/is_prime_big.h"

/* Static cache to store already found primes.
   For n up to about 10 000 the term is < 1 000 000, so this size is safe. */
#define CACHE_CAP 20000            /* must be ≥ maximum n that will be requested */

static seq_t cache[CACHE_CAP];
static size_t cache_size = 0;      /* number of primes stored */

/* Return the nth term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; undefined for n<1 */

    /* If requested term is already in the cache, return it. */
    if ((size_t)n <= cache_size) return cache[n - 1];

    /* Generate missing primes until the cache contains at least n elements. */
    seq_t cand;
    if (cache_size == 0) {
        cand = (seq_t)37;          /* first term */
    } else {
        cand = cache[cache_size - 1] + (seq_t)47;   /* next candidate in this congruence */
    }

    while (cache_size < (size_t)n) {
        /* Find the next prime in the congruence class. */
        while (!is_prime_big(cand)) cand += (seq_t)47;

        /* Store it in the cache. */
        cache[cache_size] = cand;
        cache_size++;

        /* Prepare candidate for the next iteration. */
        cand += (seq_t)47;
    }

    return cache[n - 1];
}
