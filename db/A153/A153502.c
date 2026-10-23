/*
 * A153502  Primes of the form 3*n^2 - 3*n + 11.
 * https://oeis.org/A153502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 60, u32 n <= 124, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* a(n) is the n‑th prime among the numbers 3*k^2-3*k+11, k=1,2,3,... */

static seq_t *cache = NULL;      /* cached prime values               */
static size_t cache_cap = 0;     /* allocated capacity of cache       */
static size_t cache_len = 0;     /* number of terms stored in cache   */
static idx_t next_k = 1;         /* next k value to test             */

seq_t a(idx_t n)
{
    /* Grow the cache until it contains at least n terms. */
    while (cache_len < n) {
        /* Compute the k‑th value of the quadratic form. */
        seq_t k = (seq_t)next_k;
        seq_t val = (seq_t)3 * k * k - (seq_t)3 * k + (seq_t)11;

        /* Keep only the prime values. */
        if (is_prime_big(val)) {
            /* Expand the cache when needed. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();               /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
        ++next_k;
    }

    /* Return the requested term (1‑based index). */
    return cache[n - 1];
}
