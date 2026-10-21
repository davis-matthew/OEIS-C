/*
 * A098649  Primes of the form 2(p+q) + 1, where p and q are consecutive
 *          primes.
 * https://oeis.org/A098649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 391, u32 n <= 392, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = primes of the form 2(p+q)+1 where p and q are consecutive primes.
   Computed by scanning consecutive primes and collecting the primes that satisfy
   the condition. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/next_prime.h"

/* Return the n-th term of A098649 (n >= 1). */
seq_t a(idx_t n)
{
    /* Static cache of already found terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_used = 0;

    /* Prime generator state. */
    static uint64_t prev_prime_val = 2;   /* 1st prime */
    static uint64_t next_prime_val = 3;   /* 2nd prime */
    static bool initialized = false;

    if (!initialized) {
        /* First call: set up generator. */
        cache = NULL;
        cache_cap = 0;
        cache_used = 0;
        prev_prime_val = 2;
        next_prime_val = 3;
        initialized = true;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_used < n) {
        /* Compute the candidate: r = 2 * (p + q) + 1 */
        seq_t r = (seq_t)2 * ((seq_t)prev_prime_val + (seq_t)next_prime_val) + 1;

        if (is_prime_big(r)) {
            /* Ensure enough space in the cache. */
            if (cache_used + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                if (cache_used + 1 > new_cap) new_cap = cache_used + 1;
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[cache_used++] = r;
        }

        /* Advance to the next pair of consecutive primes. */
        prev_prime_val = next_prime_val;
        next_prime_val = next_prime(prev_prime_val);
    }

    /* Return the requested term (1‑based index). */
    return cache[n - 1];
}
