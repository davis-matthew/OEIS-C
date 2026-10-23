/*
 * A142496  Primes congruent to 32 mod 51.
 * https://oeis.org/A142496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 32 modulo 51 (offset 1)
 * Range:     u8 n <= 1, u16 n <= 214, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* Returns the n-th prime of the form 51*k + 32 (with n >= 1). */
seq_t a(idx_t n)
{
    idx_t trial   = 32;   /* start with the smallest number ≡32 (mod 51) */
    idx_t count   = 0;    /* how many such primes we have found */

    while (1) {
        trial += 51;              /* next candidate of the form 51*k + 32 */
        if (is_prime(trial)) {    /* helper handles idx_t values */
            ++count;
            if (count == n)
                return (seq_t)trial;
        }
    }
}
