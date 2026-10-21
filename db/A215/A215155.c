/*
 * A215155  Primes congruent to {2, 3, 5, 7} mod 13.
 * https://oeis.org/A215155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence of primes congruent to 2, 3, 5 or 7 modulo 13. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/isPrimeTrial.h"

/* Return the n-th prime that is 2, 3, 5 or 7 (mod 13).
   The sequence starts with a(1) = 2. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t cand  = 1;          /* start just before the first candidate */

    while (count < n) {
        cand++;

        /* Skip even numbers greater than 2 (they are not prime). */
        if (cand > 2 && (cand & 1) == 0)
            continue;

        /* Fast primality test provided by the library. */
        if (!isPrimeTrial((seq_t)cand))
            continue;

        uint64_t r = cand % 13ULL;
        if (r == 2 || r == 3 || r == 5 || r == 7) {
            ++count;
        }
    }

    return (seq_t)cand;
}
