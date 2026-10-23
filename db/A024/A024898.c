/*
 * A024898  Positive integers k such that 6*k - 1 is prime.
 * https://oeis.org/A024898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer k such that 6*k-1 is prime
 * Range:     u8 n <= 27, u16 n <= 2417, u32 n <= 2418, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a static cache of found terms. It uses the
 * is_prime_big utility for primality testing on the value 6*k-1. */

#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* array of found k's */
    static size_t cache_len = 0;      /* number of terms stored */
    static size_t cache_cap = 0;      /* capacity of the array */
    static seq_t last_k = 0;          /* last k examined */

    if (n == 0) return 0;             /* undefined for offset 0 */

    if ((idx_t)cache_len >= n)        /* already have n terms */
        return cache[n - 1];

    /* Grow the cache until it contains n terms */
    while ((idx_t)cache_len < n) {
        /* Move to next candidate k */
        last_k = last_k + 1;

        /* Compute 6*k - 1 as a seq_t value */
        seq_t candidate = last_k * (seq_t)6 - (seq_t)1;

        /* Test primality of 6*k-1 */
        if (is_prime_big(candidate)) {
            /* Ensure enough room in the cache array */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache,
                            new_cap * sizeof(seq_t));
                if (!tmp) { free(cache); exit(1); }
                cache = tmp;
                cache_cap = new_cap;
            }
            /* Store the valid k */
            cache[cache_len++] = last_k;
        }
    }

    return cache[n - 1];
}
