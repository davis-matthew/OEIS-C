/*
 * A045456  Primes congruent to {1, 5} mod 7.
 * https://oeis.org/A045456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 1 or 5 modulo 7 (OEIS A045456) */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n<1, return 0 */
    idx_t count = 0;
    idx_t candidate = 5;               /* first prime of the required form */

    while (1)
    {
        if ((candidate % 7 == 1 || candidate % 7 == 5) && is_prime(candidate))
        {
            count++;
            if (count == n)
                return (seq_t)candidate;
        }
        candidate += 2;                  /* only odd candidates needed */
    }
}
