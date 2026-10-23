/*
 * A142011  Primes congruent to 7 mod 31.
 * https://oeis.org/A142011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 224, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of the primes that are ≡ 7 (mod 31).  The code
   searches for them incrementally, checking each candidate with the
   provided primality routine.  No hard‑coded table is used. */

#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

/* Return the n‑th prime congruent to 7 modulo 31, with a(1) = 7. */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; 0 is unused */
        return (seq_t)0;

    idx_t count = 0;
    idx_t candidate = 7;   /* first number ≡ 7 (mod 31) */

    while (1) {
        if (is_prime(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        candidate += 62;   /* jump to the next number ≡ 7 (mod 31) */
    }
}
