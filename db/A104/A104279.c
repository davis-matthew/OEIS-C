/*
 * A104279  Numbers n such that 2n+1 is prime and 2n-1 is not prime.
 * https://oeis.org/A104279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, push_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A104279: numbers n such that 2n+1 is prime and 2n-1 is not prime */
/* This implementation caches previously computed terms and expands the cache
   as needed. It relies on the utility functions `is_prime` and `push_seq`. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/push_seq.h"

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t size = 0;        /* number of terms stored so far */
    static idx_t cap  = 0;        /* capacity of the cache */

    /* candidate m for the next term; start after the last stored term */
    uint64_t m = (size ? (uint64_t)cache[size - 1] : 0) + 1;

    /* expand cache until we have n terms */
    while (size < n) {
        if (is_prime(2 * m + 1) && !is_prime(2 * m - 1)) {
            /* store the new term; push_seq resizes the array as required */
            push_seq(&cache, &cap, &size, (seq_t)m);
        }
        ++m;
    }

    return cache[n - 1];
}
