/*
 * A171386  The characteristic function of 2 and 3: 1 if n is prime such that
 *          either n-1 or n+1 is prime, else 0.
 * https://oeis.org/A171386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 1 if n is prime and either n-1 or n+1 is prime, otherwise
 *            0
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;           /* 1 is not prime */

    if (!is_prime(n))
        return (seq_t)0;           /* not a prime */

    /* now n is prime */
    if (is_prime(n - 1) || is_prime(n + 1))
        return (seq_t)1;

    return (seq_t)0;
}
