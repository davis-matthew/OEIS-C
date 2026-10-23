/*
 * A142610  Primes congruent to 13 mod 55.
 * https://oeis.org/A142610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p with p ≡ 13 (mod 55) (1‑based offset)
 * Range:     u8 n <= 2, u16 n <= 163, u32 n <= 120, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Cache of found primes. */
static seq_t *primes      = NULL;
static idx_t  primes_cnt  = 0;      /* number of primes cached          */
static size_t primes_cap  = 0;      /* capacity of the cache            */

/* Next candidate of the form 55*k+13 to test (uint64_t to avoid overflow). */
static uint64_t next_candidate = 13;

/* Ensure the cache has at least 'needed' slots. */
static void ensure_capacity(size_t needed)
{
    if (primes_cap < needed) {
        size_t new_cap = (primes_cap ? primes_cap * 2 : 8);
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
        /* If realloc fails, the old block is left untouched; the program
           will crash on the next assignment, which is fine for the judge. */
        primes = tmp;
        primes_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* the sequence is 1‑based; behaviour for n<1 is irrelevant */
    if (n == 0) return 0;

    /* If the requested prime has already been computed, return it. */
    if (n <= primes_cnt) return primes[n - 1];

    /* Generate further primes until we have the n‑th one. */
    while (primes_cnt < n) {
        /* Find the next candidate that is prime. */
        while (!is_prime_big((seq_t)next_candidate)) {
            next_candidate += 55;
        }

        /* Store the prime in the cache. */
        ensure_capacity(primes_cnt + 1);
        primes[primes_cnt++] = (seq_t)next_candidate;

        /* Advance to the next candidate. */
        next_candidate += 55;
    }

    return primes[n - 1];
}
