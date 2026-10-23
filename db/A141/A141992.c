/*
 * A141992  Primes congruent to 16 mod 29.
 * https://oeis.org/A141992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 228, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th prime congruent to 16 (mod 29) by iterating the
   arithmetic progression 16 + 29*k and testing each value for primality.
   This uses the helper is_prime(idx_t) defined in the lib.  The offset is
   1: a(1) = 103, a(2) = 277, … */

#include <stdbool.h>   /* for bool */
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1 */

    idx_t count     = 0;              /* number of primes found so far */
    idx_t candidate = 16;             /* start of the AP 16 + 29*k */

    while (1) {
        candidate += 29;              /* next number in the progression */
        if (is_prime(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
    }
}
