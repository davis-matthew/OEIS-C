/*
 * A045431  Primes congruent to {1, 3, 4} mod 7.
 * https://oeis.org/A045431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime that is congruent to 1, 3 or 4 (mod 7)
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for n=0, but return 0 to avoid UB */
        return (seq_t)0;
    }

    uint64_t count   = 0;   /* how many primes of the desired class we have found */
    uint64_t candidate = 3; /* first odd prime candidate */

    while (count < n) {
        /* check candidate is prime */
        if (is_prime(candidate)) {
            uint64_t r = candidate % 7;
            if (r == 1 || r == 3 || r == 4) {
                ++count;
                if (count == n) {
                    return (seq_t)candidate;
                }
            }
        }
        candidate += 2;     /* skip even numbers only */
    }

    /* Should never reach here */
    return (seq_t)0;
}
