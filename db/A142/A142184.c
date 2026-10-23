/*
 * A142184  Primes congruent to 3 mod 40.
 * https://oeis.org/A142184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime number p such that p ≡ 3 (mod 40)
 * Range:     u8 n <= 4, u16 n <= 409, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function generates primes in increasing order and caches them, so
   repeated calls are fast and independent of order. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static seq_t *cache      = NULL;  /* stored terms */
static size_t cache_len  = 0;     /* number of stored terms */
static size_t cache_cap  = 0;     /* current capacity of cache */
static idx_t  next_cand   = 3;    /* next candidate ≡ 3 (mod 40) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1; n ≥ 1 is expected */

    /* Grow cache until it contains at least n terms */
    while (cache_len < n) {
        if (is_prime(next_cand)) {
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : (cache_cap * 2);
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;          /* allocation failure */
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_cand;   /* store value */
        }
        next_cand += 40;                            /* next 3 mod 40 candidate */
    }

    return cache[n - 1];
}
