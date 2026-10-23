/*
 * A141932  Primes congruent to 7 mod 25.
 * https://oeis.org/A141932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 323, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate numbers ≡7 (mod 25) and test primality with is_prime(idx_t). */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* Return the n‑th prime that is congruent to 7 modulo 25.
   n ≥ 1.  The function is deterministic and works for any order of calls. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n < 1 */

    idx_t count = 0;                      /* how many primes found so far   */
    idx_t candidate = 7;                  /* first number ≡7 (mod 25)       */

    while (true) {
        if (is_prime(candidate)) {
            ++count;
            if (count == n) return (seq_t)candidate;
        }
        candidate += 25;                  /* next number ≡7 (mod 25)        */
    }
}
