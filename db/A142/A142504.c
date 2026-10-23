/*
 * A142504  Primes congruent to 46 mod 51.
 * https://oeis.org/A142504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 212, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: candidates of the form 51*k+46 are tested for primality. */

#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* The sequence starts at a(1) = 97.  For n = 0 we simply return 0. */
    if (n == 0)
        return 0;

    idx_t count = 0;          /* number of suitable primes found so far   */
    idx_t candidate = 46;     /* start below the first prime (97)         */

    /* Search until we have found the n‑th prime ≡ 46 (mod 51). */
    while (1)
    {
        candidate += 51;                      /* next candidate ≡ 46 (mod 51) */
        if (is_prime(candidate))              /* primality test   */
        {
            ++count;
            if (count == n)
                return (seq_t)candidate;      /* cast to seq_t for output   */
        }
    }
}
