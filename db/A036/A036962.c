/*
 * A036962  Primes without {8, 9} as digits.
 * https://oeis.org/A036962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 2575, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bad_digit, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the nth prime that contains no digit 8 or 9 */
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/bad_digit.h"

/* Compute the nth term of OEIS A036962 */
seq_t a(idx_t n)
{
    if (n < 1)                /* undefined for n < 1, return 0 */
        return (seq_t)0;

    idx_t count    = 0;
    idx_t candidate = 2;

    while (count < n)
    {
        if (!bad_digit(candidate) && is_prime(candidate))
        {
            ++count;            /* found a valid prime */
            if (count == n)
                break;
        }
        ++candidate;
    }
    return (seq_t)candidate;
}
