/*
 * A188130  Primes p such that 6p+1 divides the Mersenne number
 *          M(p)=A000225(p).
 * https://oeis.org/A188130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 55, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A188130: primes p such that 6p+1 divides 2^p−1. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but return 0 */

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t candidate = 5; /* next number ≡1 (mod 4) to test */

    /* Return cached value if already computed */
    if (cache_len >= (size_t)n) {
        return cache[n - 1];
    }

    /* Ensure cache capacity */
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we reach the requested index */
    while (cache_len < (size_t)n) {
        /* candidate is always ≡1 (mod 4) because we advance by 4 */
        if (is_prime(candidate)) {
            seq_t d = (seq_t)(6ULL * candidate + 1ULL);
            seq_t rem = mod_pow((seq_t)2, (uint64_t)candidate, d);
            if (rem == (seq_t)1) {
                /* Store the prime in the cache */
                cache[cache_len++] = (seq_t)candidate;

                /* Grow the cache if full */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) return 0;
                    cache = tmp;
                    cache_cap = new_cap;
                }
            }
        }
        candidate += 4; /* next ≡1 (mod 4) candidate */
    }

    return cache[n - 1];
}
