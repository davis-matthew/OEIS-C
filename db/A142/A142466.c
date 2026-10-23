/*
 * A142466  Primes congruent to 27 mod 50.
 * https://oeis.org/A142466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 326, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime congruent to 27 (mod 50) by scanning over
   candidates.  The sequence is defined as the list of primes p
   satisfying p ≡ 27 (mod 50), indexed starting from 1.  */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* not a valid index, return 0 */

    idx_t count    = 0;              /* number of primes found so far   */
    idx_t candidate = 27;            /* candidate ≡ 27 (mod 50)         */

    /* Keep generating candidates until we have found the n-th prime. */
    while (count < n) {
        candidate += 50;              /* next number ≡ 27 (mod 50)       */
        if (is_prime(candidate)) {    /* primality test                  */
            ++count;
        }
    }

    return (seq_t)candidate;         /* cast to the sequence type      */
}
