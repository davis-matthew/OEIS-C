/*
 * A216315  Primes congruent to 1 mod 59.
 * https://oeis.org/A216315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 83, u32 n <= 84, u64 n <= 3000, u128 n <= 3000,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime congruent to 1 modulo 118 (offset 1).  */

/* Uses a simple incremental search, caching all primes found.  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Static cache of primes already found. */
static seq_t *prime_cache = NULL;   /* array of primes */
static size_t prime_cache_cap = 0;  /* allocated capacity */
static size_t prime_cache_cnt = 0;  /* number of primes stored */
static idx_t next_candidate_k = 1;   /* k for the next 118*k+1 candidate */

/* Grow the prime cache when it runs out of space.  */
static void grow_prime_cache(void)
{
    size_t new_cap = prime_cache_cap == 0 ? 64 : prime_cache_cap * 2;
    seq_t *new_ptr = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();                /* out of memory */
    prime_cache = new_ptr;
    prime_cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for offset 1 */

    /* Generate new primes until we have at least n of them. */
    while (prime_cache_cnt < n) {
        /* Candidate prime: p = 118*k + 1.  */
        seq_t cand = (seq_t)118 * (seq_t)next_candidate_k + (seq_t)1;

        if (is_prime_big(cand)) {
            if (prime_cache_cnt >= prime_cache_cap) grow_prime_cache();
            prime_cache[prime_cache_cnt++] = cand;
        }
        ++next_candidate_k;               /* advance to next k */
    }

    return prime_cache[n - 1];
}
