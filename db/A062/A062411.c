/*
 * A062411  a(n) = (-1)^(p-1)*(p-1)! + 1 where p = prime(n).
 * https://oeis.org/A062411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (-1)^(p-1)*(p-1)! + 1 where p = prime(n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For all primes p>2 the exponent is even, so a(n) = (p-1)! + 1.  */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* a(1) corresponds to p = 2, giving 0; all other primes are odd. */
    idx_t p = get_prime(n);
    if (p == 2)               /* first term: (-1)^1*1! + 1 = 0 */
        return (seq_t)0;

    seq_t fact = 1;
    for (idx_t i = 2; i < p; ++i)
        fact *= (seq_t)i;

    return fact + 1;
}
