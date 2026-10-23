/*
 * A154624  Primes congruent to 34 mod 71.
 * https://oeis.org/A154624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime p with p ≡ 34 (mod 71)
 * Range:     u8 none, u16 n <= 91, u32 n <= 66, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code searches candidates of the form 71*k + 34 and tests
   each for primality using is_prime_big. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t k    = 0;          /* candidate index: p = 71*k + 34 */

    while (true) {
        /* Build the candidate prime.  All arithmetic is done in seq_t. */
        seq_t p = ((seq_t)k) * 71 + 34;

        /* Test primality.  is_prime_big works for both fixed‑size
           and arbitrary‑precision seq_t. */
        if (is_prime_big(p)) {
            ++count;
            if (count == n) {
                return p;
            }
        }
        ++k; /* next candidate */
    }
}
