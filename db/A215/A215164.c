/*
 * A215164  Primes congruent to {1, 4} mod 13.
 * https://oeis.org/A215164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   nth prime congruent to 1 or 4 modulo 13
 * Range:     u8 n <= 9, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: iterate over numbers of the form 13*k+r (r∈{1,4}),
   test each for primality with is_prime, and return the nth such prime. */
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;

    for (;; ++k) {
        /* Candidate 13*k + 1 */
        idx_t cand1 = 13 * k + 1;
        if (cand1 >= 2 && is_prime(cand1)) {
            ++count;
            if (count == n) return (seq_t)cand1;
        }
        /* Candidate 13*k + 4 */
        idx_t cand2 = 13 * k + 4;
        if (cand2 >= 2 && is_prime(cand2)) {
            ++count;
            if (count == n) return (seq_t)cand2;
        }
    }
}
