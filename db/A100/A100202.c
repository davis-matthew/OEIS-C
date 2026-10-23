/*
 * A100202  Primes of the form 13*k + 3.
 * https://oeis.org/A100202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime of the form 13*k + 3
 * Range:     u8 n <= 4, u16 n <= 545, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* seq_t and idx_t are defined by the build configuration */
seq_t a(idx_t n)
{
    /* Current count of primes found of the required form. */
    idx_t count = 0;
    /* Current k such that candidate = 13*k + 3. Start with k = 0 → 3. */
    idx_t k = 0;

    /* Loop until we've located the nth such prime. */
    while (true) {
        /* Compute the current candidate; use uint64_t to avoid overflow of the
           arithmetic before the cast to seq_t. */
        uint64_t cand = 13 * k + 3;

        /* Check primality using the supplied helper. */
        if (is_prime(cand)) {
            ++count;
            if (count == n) {
                return (seq_t)cand;
            }
        }
        ++k;
    }
}
