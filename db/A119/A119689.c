/*
 * A119689  Numbers n such that the sum of the largest distinct prime divisor
 *          and the smallest distinct prime divisor is a prime.
 * https://oeis.org/A119689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _smallest_prime_factor, gpf, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n such that the sum of the largest distinct prime divisor and
   the smallest distinct prime divisor is a prime. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/_smallest_prime_factor.h"
#include "../utils/gpf.h"

/* Return the n-th term of A119689 (offset 1). */
seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *cache = NULL;
    static size_t len   = 0;
    static size_t cap   = 0;

    /* Candidate numbers to test. We start with 2 and increment. */
    static idx_t tested = 2;

    while (len < (size_t)n) {
        uint64_t cand = (uint64_t)tested;

        /* Skip 0 and 1, they have no prime divisors. */
        if (cand >= 2) {
            uint64_t spf = _smallest_prime_factor(cand);
            if (spf == 0)  /* cand is prime */
                spf = cand;
            uint64_t lpf = gpf(cand);

            /* Check if the sum of the distinct prime divisors is prime. */
            if (is_prime_small((seq_t)spf + (seq_t)lpf)) {
                if (len == cap) {
                    cap = cap ? (cap << 1) : 8;
                    cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                }
                cache[len++] = (seq_t)cand;
            }
        }

        ++tested;
    }

    return cache[n - 1];
}
