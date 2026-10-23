/*
 * A116512  a(n) is the number of positive integers each of which is <= n and
 *          is divisible by exactly one prime dividing n (but is coprime to
 *          every other prime dividing n). a(1) = 0.
 * https://oeis.org/A116512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n, d is prime power} phi(n/d)
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where phi is Euler's totient function. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* safety, though n >= 1 by definition */

    idx_t const n_const = n;       /* keep original value for division   */
    seq_t result = 0;

    idx_t temp = n;                /* work on a copy for factor extraction */

    /* trial division to find all distinct prime factors of n */
    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            /* p is a prime factor – iterate over all powers p^e that
               divide n_const */
            idx_t power = p;
            for (;;)
            {
                if (n_const % power == 0)
                    result += phi(n_const / power);

                /* next power, avoid overflow */
                if (power > n_const / p)
                    break;
                power *= p;
            }

            /* remove this prime factor completely from temp */
            while (temp % p == 0) temp /= p;
        }
    }

    /* if a prime factor remains > sqrt(n) */
    if (temp > 1)
    {
        idx_t p = temp;
        idx_t power = p;
        for (;;)
        {
            if (n_const % power == 0)
                result += phi(n_const / power);

            if (power > n_const / p)
                break;
            power *= p;
        }
    }

    return result;
}
