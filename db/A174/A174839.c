/*
 * A174839  Lists of 4 adjacent primes such that the difference between the
 *          highest minus the lowest = 8.
 * https://oeis.org/A174839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 116, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n-th prime appearing in any quadruple of four
   consecutive primes with a maximum gap of 8.  The sequence lists all
   primes p1 < p2 < p3 < p4 that are consecutive primes and satisfy
   p4 - p1 = 8.  For each such quadruple the four primes are output
   in order, and the terms a(1), a(2), ... are taken from this list. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* Generate the first four primes */
    seq_t q[4];
    q[0] = 2;
    q[1] = (seq_t)next_prime(2);
    q[2] = (seq_t)next_prime((uint64_t)q[1]);
    q[3] = (seq_t)next_prime((uint64_t)q[2]);

    idx_t count = 0;

    /* Check the initial quadruple */
    if ((q[3] - q[0]) == 8) {
        for (int i = 0; i < 4; ++i) {
            ++count;
            if (count == n) return q[i];
        }
    }

    /* Slide a window of four consecutive primes, looking for gap 8 */
    while (1) {
        uint64_t nextp = next_prime((uint64_t)q[3]);

        q[0] = q[1];
        q[1] = q[2];
        q[2] = q[3];
        q[3] = (seq_t)nextp;

        if ((q[3] - q[0]) == 8) {
            for (int i = 0; i < 4; ++i) {
                ++count;
                if (count == n) return q[i];
            }
        }
    }
}
