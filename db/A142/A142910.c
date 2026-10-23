/*
 * A142910  Primes congruent to 38 mod 63.
 * https://oeis.org/A142910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime congruent to 38 modulo 63 (offset 1)
 * Range:     u8 n <= 2, u16 n <= 181, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 1. */
    if (n == 0)
        return (seq_t)0;

    idx_t candidate = 38;   /* Start at 38; the next residue will be 101. */
    idx_t found     = 0;    /* Number of qualifying primes found so far. */

    while (found < n) {
        candidate += 63;                 /* Next number ≡ 38 (mod 63) */
        if (is_prime(candidate)) {       /* Check primality */
            ++found;
            if (found == n)
                return (seq_t)candidate; /* Return the nth prime */
        }
    }

    return (seq_t)0;                       /* Unreachable, keeps the function
                                            * well‑defined for all cases. */
}
