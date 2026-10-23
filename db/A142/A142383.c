/*
 * A142383  Primes congruent to 32 mod 47.
 * https://oeis.org/A142383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 32 (mod 47)
 * Range:     u8 n <= 2, u16 n <= 152, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* The sequence is defined as the n‑th prime p such that p ≡ 32 (mod 47). */
    /* We iterate over all numbers congruent to 32 modulo 47 and test each for primality. */
    idx_t candidate = 32;        /* first number ≡ 32 (mod 47) */
    idx_t count    = 0;          /* how many suitable primes have been found */

    while (count < n) {
        if (is_prime(candidate)) {
            count++;
            if (count == n) {
                return (seq_t)candidate;   /* found the n‑th prime */
            }
        }
        candidate += 47;      /* next number with the same residue */
    }

    /* Should never reach here for n >= 1 */
    return (seq_t)0;
}
