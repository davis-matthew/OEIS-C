/*
 * A045400  Primes congruent to {0, 1, 3, 5} mod 7.
 * https://oeis.org/A045400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of primes congruent to 0, 1, 3, or 5 (mod 7) */
/* Computation: iterate over odd integers, test for primality and check the modulo residue. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/*  seq_t – type for sequence values (unsigned integer or big integer)              */
/*  idx_t   – index type, usually uint64_t                                            */

seq_t a(idx_t n)
{
    if (n == 0U) {
        /* The sequence is offset 1; n==0 is outside the domain. */
        return (seq_t)0;
    }

    idx_t count = 0U;
    /* Start with the first odd prime > 2. */
    idx_t candidate = 3U;

    while (true) {
        /* Test the modulo class only for the few admissible residues. */
        uint64_t r = candidate % 7U;
        if (r == 0U || r == 1U || r == 3U || r == 5U) {
            if (is_prime(candidate)) {
                ++count;
                if (count == n) {
                    return (seq_t)candidate;
                }
            }
        }
        /* Next odd integer. */
        candidate += 2U;
    }
}
