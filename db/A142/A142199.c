/*
 * A142199  Primes congruent to 2 mod 41.
 * https://oeis.org/A142199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 167, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the n‑th prime that is congruent to 2 modulo 41.
   The sequence a(n) is defined as: a(n) = p_n where p_n ≡ 2 (mod 41). */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t count     = 0;          /* number of suitable primes found so far   */
    idx_t candidate = 2;          /* 41*0 + 2, the first admissible number   */

    while (1) {
        if (is_prime(candidate)) {
            count++;
            if (count == n)
                return (seq_t)candidate;
        }
        candidate += 41;          /* next number of the form 41*k + 2       */
    }
}
