/*
 * A138362  Primes of the form k^2 + 11.
 * https://oeis.org/A138362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 19, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A138362: a(n) = the n-th prime that can be written as k^2 + 11. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Computes the n-th term of sequence A138362.  Calls to a() are
   memoised and work in any order. */
seq_t a(idx_t n)
{
    /* Handle the trivial out-of-range case. */
    if (n == 0) return 0;

    /* Static cache of primes of the required form. */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    /* Next k to test; starts at 0 and increases monotonically. */
    static idx_t next_k   = 0;

    /* Grow cache until we have at least n entries. */
    while (cache_len < (size_t)n) {
        /* Ensure sufficient backing storage. */
        if (cache_cap == 0) {
            cache_cap = 16;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            if (!cache) return 0; /* allocation failure */
        } else if (cache_len == cache_cap) {
            cache_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!tmp) return 0; /* allocation failure */
            cache = tmp;
        }

        /* Test k^2 + 11 for primality. */
        seq_t candidate = (seq_t)next_k * (seq_t)next_k + (seq_t)11;
        if (is_prime_big(candidate)) {
            cache[cache_len++] = candidate;
        }
        ++next_k;
    }

    /* Convert from 1‑based to 0‑based index. */
    return cache[n - 1];
}
