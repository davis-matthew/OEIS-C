/*
 * A142182  Primes congruent to 38 mod 39.
 * https://oeis.org/A142182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 272, u32 n <= 273, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 38 (mod 39)
   a(n) returns the nth prime p such that p % 39 == 38,
   with a(1) = 233. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Cache of found primes */
static seq_t *prime_cache   = NULL;
static size_t prime_len     = 0;
static size_t prime_cap     = 0;

/* Next candidate of the form 39*k + 38, starting from 38 */
static uint64_t next_candidate = 38;

/* Smallest positive integer > 0 that fits in seq_t (used as an upper bound). */
static const uint64_t max_seq_t_value = (uint64_t)((seq_t)-1);

/* Ensure the cache can hold at least one more element. */
static void ensure_cache_capacity(void)
{
    if (prime_len >= prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – clean up and leave the cache empty. */
            free(prime_cache);
            prime_cache = NULL;
            prime_cap   = 0;
            prime_len   = 0;
            return;
        }
        prime_cache = tmp;
        prime_cap   = new_cap;
    }
}

/* Return the nth prime congruent to 38 (mod 39), with n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of range, value irrelevant */

    while (prime_len < n) {
        /* Stop early if the candidate cannot fit into seq_t. */
        if (next_candidate > max_seq_t_value) {
            /* No more representable primes – return 0 or abort. */
            return (seq_t)0;
        }

        ensure_cache_capacity();
        if (!prime_cache) return (seq_t)0;   /* allocation failed */

        /* Test the candidate for primality and store it if it is prime. */
        if (is_prime_big((seq_t)next_candidate)) {
            prime_cache[prime_len++] = (seq_t)next_candidate;
        }
        next_candidate += 39;   /* Next number of the form 39*k + 38 */
    }

    return prime_cache[n - 1];
}
