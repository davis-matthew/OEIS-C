/*
 * A179336  Primes containing at least one prime digit in base 10.
 * https://oeis.org/A179336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the nth prime that contains at least one decimal digit
 *            that is itself prime (2, 3, 5, or 7)
 * Range:     u8 n <= 43, u16 n <= 6140, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_prime_digit, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/has_prime_digit.h"

seq_t a(idx_t n)
{
    /* n >= 1 (OEIS offset).  We count primes that have at least one
       prime digit and return the nth such prime. */
    idx_t count = 0;
    idx_t candidate = 2;

    for (;; ++candidate)
    {
        if (is_prime(candidate) && has_prime_digit(candidate))
        {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
    }
}
