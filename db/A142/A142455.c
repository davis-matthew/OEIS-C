/*
 * A142455  Primes congruent to 48 mod 49.
 * https://oeis.org/A142455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 150, u32 n <= 151, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the n‑th prime p such that p % 49 == 48.
   Uses incremental search with memoisation. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* we use the supplied primality test */

/* Static storage for memoised primes. */
static seq_t *primes = NULL;      /* Array of found primes. */
static size_t  primes_len  = 0;   /* Number of primes stored. */
static size_t  primes_cap  = 0;   /* Allocated capacity of the array. */
static idx_t   next_k      = 1;   /* Multiplier for the next candidate (49*k+48). */

/* Ensure the prime array has at least 'needed' slots. */
static void ensure_capacity(size_t needed)
{
    if (primes_cap < needed) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 16;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* Out of memory. */
        primes = tmp;
        primes_cap = new_cap;
    }
}

/* Return the n‑th prime p with p % 49 == 48, 1 ≤ n. */
seq_t a(idx_t n)
{
    /* Generate primes until we have at least n of them. */
    while (primes_len < n) {
        idx_t candidate_idx = 49 * next_k + 48;                /* Candidate as idx_t. */
        seq_t candidate = (seq_t)candidate_idx;                /* Convert to seq_t (may wrap for tiny types). */
        if (is_prime_big(candidate)) {
            ensure_capacity(primes_len + 1);
            primes[primes_len++] = candidate;
        }
        next_k++;
    }
    return primes[n - 1];
}
