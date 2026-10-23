/*
 * A182357  Primes of the form 2^k + k^2 + 2.
 * https://oeis.org/A182357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th prime of the form 2^k + k^2 + 2 (k ≥ 0)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   is_prime_big, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

/* Helpers from the utils library */
#include "../utils/pow_uint.h"      /* seq_t pow_uint(uint64_t base, idx_t exp) */
#include "../utils/is_prime_big.h"  /* bool is_prime_big(seq_t n) */

/* A static cache for the first primes found.  The problem statement
   guarantees that around 10 000 terms are requested; 20 000 is more
   than enough and keeps the implementation simple. */
#define MAX_PRIMES 20000
static seq_t cached_primes[MAX_PRIMES];
static idx_t cached_count = 0;   /* number of primes stored in the cache */
static idx_t next_k = 0;         /* next k to test for the formula */

/* Return the n-th term (n ≥ 1) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined; caller never uses this */

    /* Extend the cache until we have at least n primes */
    while (cached_count < n)
    {
        /* Compute candidate = 2^k + k^2 + 2 */
        seq_t pow_term = pow_uint(2, next_k);          /* 2^k */
        seq_t k_val = (seq_t)next_k;                  /* cast to seq_t */
        seq_t k_sq = k_val * k_val;                   /* k^2 */
        seq_t candidate = pow_term + k_sq + 2;        /* 2^k + k^2 + 2 */

        /* Test primality */
        if (is_prime_big(candidate))
        {
            assert(cached_count < MAX_PRIMES); /* safety guard */
            cached_primes[cached_count++] = candidate;
        }
        ++next_k;                               /* move to the next k */
    }

    return cached_primes[n - 1];
}
