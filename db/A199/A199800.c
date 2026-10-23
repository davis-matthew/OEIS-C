/*
 * A199800  Number of ways to write n = p+q with p, 6q-1 and 6q+1 all prime.
 * https://oeis.org/A199800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations n = p + q with p prime, 6*q - 1
 *            prime and 6*q + 1 prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation iterates over all possible q (1 ≤ q ≤ n-2),
   checks the primality of p = n - q and of 6*q-1, 6*q+1 using the
   provided is_prime(idx_t) helper, and counts the qualifying pairs. */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 3)            /* n too small for any representation */
        return 0;

    idx_t count = 0;

    for (idx_t q = 1; q <= n - 2; ++q)
    {
        idx_t p = n - q;

        if (!is_prime(p))
            continue;

        idx_t m1 = 6 * q - 1;
        idx_t m2 = 6 * q + 1;

        if (is_prime(m1) && is_prime(m2))
            ++count;
    }

    return (seq_t)count;
}
