/*
 * A142914  Primes congruent to 44 mod 63.
 * https://oeis.org/A142914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 169, u32 n <= 182, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 44 modulo 63. */
/* Each call returns the nth prime ≡ 44 (mod 63), with a(1) = 107. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Memoised cache of primes found so far. */
    static seq_t *cache = NULL;          /* array of primes      */
    static size_t cache_size = 0;        /* number of primes     */
    static size_t cache_cap  = 0;        /* allocated capacity   */
    static seq_t next_candidate = 44;    /* first number ≡ 44 (mod 63) */

    if (n < 1)                  /* offset is 1 – undefined for n=0 */
        return 0;

    /* Grow the cache until it contains at least n primes. */
    while (cache_size < (size_t)n) {
        /* Test if the current candidate is prime. */
        if (is_prime_big((seq_t)next_candidate)) {
            /* Ensure there is room for a new prime entry. */
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                cache = tmp;
                cache_cap = new_cap;
            }
            /* Store the prime. */
            cache[cache_size++] = next_candidate;
        }
        /* Move to the next number ≡ 44 (mod 63). */
        next_candidate += 63;
    }

    /* Return the requested term (1‑based index). */
    return cache[n - 1];
}
