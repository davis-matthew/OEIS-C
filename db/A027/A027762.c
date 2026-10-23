/*
 * A027762  Denominator of Sum_{p prime, p-1 divides 2*n} 1/p.
 * https://oeis.org/A027762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = product of primes p such that (p-1) divides 2*n
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Undefined offset; a(0) is not part of the sequence. */
    if (n == 0) return 0;

    idx_t m = n + n;          /* 2*n */
    seq_t prod = 1;
    idx_t p;

    for (idx_t d = 1; d*d <= m; ++d)
    {
        if (m % d == 0)
        {
            p = d + 1;
            if (is_prime(p))
                prod *= (seq_t)p;

            idx_t d2 = m / d;
            if (d2 != d)
            {
                p = d2 + 1;
                if (is_prime(p))
                    prod *= (seq_t)p;
            }
        }
    }

    return prod;
}
