/*
 * A142013  Primes congruent to 9 mod 31.
 * https://oeis.org/A142013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 218, u32 n <= 44, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime congruent to 9 modulo 31 */
/* This implementation searches for primes of the form 31*k+9
   and caches them so that subsequent calls are fast.  The cache
   grows lazily as larger indices are requested.  The function
   works for all n ≥ 1 and returns the nth term exactly when the
   value fits in the target seq_t type. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t *prime_cache = NULL;   /* cached primes */
static size_t cache_len   = 0;      /* number of primes stored */
static size_t cache_cap   = 0;      /* current capacity of prime_cache */
static idx_t   next_k      = 0;      /* next k value to test (candidate = 31*k+9) */

static void resize_cache(void) {
    if (cache_cap == 0) {
        cache_cap = 128;
        prime_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_len == cache_cap) {
        cache_cap *= 2;
        prime_cache = (seq_t *)realloc(prime_cache, cache_cap * sizeof(seq_t));
    }
}

seq_t a(idx_t n) /* returns the nth prime congruent to 9 (mod 31) */
{
    if (n == 0) return (seq_t)0;               /* undefined offset, harmless */

    while (cache_len < n) {
        /* Use 64‑bit arithmetic for the candidate; it stays ≤ 2^64 for n≈10⁴. */
        idx_t cand64 = 31 * next_k + 9;
        seq_t cand = (seq_t)cand64;

        if (is_prime_big(cand)) {
            resize_cache();
            prime_cache[cache_len++] = cand;
        }
        next_k++;
    }
    return prime_cache[n - 1];
}
