/*
 * A144419  Primes of the form prime(k) - 2*k (terms can be repeated).
 * https://oeis.org/A144419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(k) – 2*k for the k-th prime values that are prime.
 *            This function returns the n-th term of OEIS A144419 with offset
 *            1
 * Range:     u8 n <= 31, u16 n <= 1597, u32 n <= 716, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* The following utilities are available in the build environment. */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */
#include "../utils/get_prime.h"     /* uint32_t get_prime(idx_t k); */

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* Static state for memoisation. */
    static seq_t *cache = NULL;    /* Array of computed terms. */
    static size_t cache_len = 0;   /* Number of terms stored. */
    static size_t cache_cap = 0;   /* Allocated capacity. */
    static idx_t next_k = 1;       /* Next k to examine. */

    /* Validate input – in this project n is always >= 1. */
    if (n < 1)
        return 0;

    /* Grow the cache until we have at least n terms. */
    while (cache_len < (size_t)n) {
        /* Compute prime(k) - 2*k in a signed intermediate to avoid
           underflow for small k. */
        int64_t temp = (int64_t)get_prime(next_k);
        temp -= (int64_t)2 * (int64_t)next_k;

        if (temp > 1 && is_prime_big((seq_t)temp)) {
            /* Ensure capacity for the new term. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_ptr) {
                    /* Out of memory – return 0 as an error indicator. */
                    return 0;
                }
                cache = new_ptr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)temp;
        }
        next_k++;  /* Advance to next k. */
    }

    /* Return the requested term. */
    return cache[n - 1];
}
