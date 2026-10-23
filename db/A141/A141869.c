/*
 * A141869  Primes congruent to 2 mod 19.
 * https://oeis.org/A141869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 303, u32 n <= 304, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: primes congruent to 2 mod 19, searched incrementally. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* n must be >= 1. For n==0 we return 0 as a harmless fallback. */
    if (n == 0) {
        return 0;
    }

    seq_t cand = 2;          /* first candidate */
    idx_t  count = 0;        /* number of qualifying primes found */

    for (;; ++cand) {
        /* only numbers congruent to 2 modulo 19 are candidates */
        if ((cand % (seq_t)19) == (seq_t)2) {
            if (is_prime_big(cand)) {
                ++count;
                if (count == n) {
                    return cand;
                }
            }
        }
    }
}
