/*
 * A142663  Primes congruent to 53 mod 56.
 * https://oeis.org/A142663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 279, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of the primes p such that p ≡ 53 (mod 56).  
   This function searches sequentially for the n‑th such prime. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* The helper is_prime is available from the build utilities. */
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for n = 0, return 0 for safety */
        return (seq_t)0;

    idx_t found = 0;           /* number of primes seen so far          */
    uint64_t candidate = 53;    /* the first prime congruent to 53 mod 56 */

    while (true) {
        if (is_prime(candidate)) {
            ++found;
            if (found == n)
                return (seq_t)candidate;
        }
        candidate += 56;        /* next number that is 53 (mod 56) */
    }

    /* Unreachable, but keeps the compiler happy */
    return (seq_t)0;
}
