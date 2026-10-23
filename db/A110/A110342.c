/*
 * A110342  a(1) = 1; a(n) = product of n successive primes beginning with n
 *          if n is prime else product of n successive composite numbers
 *          beginning with n.
 * https://oeis.org/A110342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of n successive primes beginning with n if n is
 *            prime, otherwise product of n successive composite numbers
 *            beginning with n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Handle the special case a(1) = 1. */
    if (n == 1)
        return (seq_t)1;

    /* Determine whether n itself is prime. */
    bool start_is_prime = is_prime((uint64_t)n);

    seq_t result = 1;
    idx_t count = 0;

    /* Scan numbers starting from n (which is ≥ 2 here). */
    for (idx_t m = n; ; ++m)
    {
        if (start_is_prime)
        {
            if (is_prime((uint64_t)m))
            {
                result *= (seq_t)m;
                ++count;
                if (count == n)
                    break;
            }
        }
        else
        {
            /* Composite numbers are defined as integers >1 that are not prime. */
            if (!is_prime((uint64_t)m))
            {
                result *= (seq_t)m;
                ++count;
                if (count == n)
                    break;
            }
        }
    }
    return result;
}
