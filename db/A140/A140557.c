/*
 * A140557  Primes p such that p + 14 is composite.
 * https://oeis.org/A140557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = nth prime p such that p + 14 is composite
 * Range:     u8 n <= 37, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t count      = 0;   /* number of qualifying primes found */
    idx_t candidate  = 2;   /* current integer to test */

    while (1) {
        if (is_prime(candidate)) {
            /* p + 14 must be composite */
            if (!is_prime(candidate + 14)) {
                count++;
                if (count == n)
                    return (seq_t)candidate;
            }
        }

        /* skip even numbers after 2 */
        if (candidate == 2)
            candidate = 3;
        else
            candidate += 2;
    }
}
