/*
 * A142647  Primes congruent to 15 mod 56.
 * https://oeis.org/A142647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th prime congruent to 15 modulo 56. For n ≥ 1 the
 *            sequence is the increasing list of primes p such that p ≡ 15
 *            (mod 56)
 * Range:     u8 n <= 3, u16 n <= 270, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm tests successive candidates of the form 56·k+15
   and counts how many of them are prime until the desired rank is
   reached.  The routine is independent of previous calls, so it
   works correctly in any call order. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* For n < 1 the OEIS offset is irrelevant; return 0. */
    if (n == 0)
        return (seq_t)0;

    idx_t k = 0;          /* index of the current candidate 56·k+15 */
    seq_t count = 0;      /* number of primes found so far      */

    for (;;) {
        /* Candidate prime of the required form. */
        seq_t candidate = (seq_t)(56 * k + 15);

        /* Primality test for the candidate. */
        if (isPrimeGeneric(candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }
        ++k;
    }
}
