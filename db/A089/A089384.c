/*
 * A089384  Greatest squarefree proper divisor of n, a(1) = 1.
 * https://oeis.org/A089384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest squarefree proper divisor of n Implementation:
 *            factor n; if squarefree use product of all primes except the
 *            smallest; otherwise use the product of distinct primes (the
 *            radical)
 * Range:     u8 n <= 454, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"   /* provides factor_u64 */

seq_t a(idx_t n)
{
    if (n == 1)                       /* a(1) = 1 by definition */
        return (seq_t)1;

    /* Factorization: primes[0..cnt-1] contain distinct primes,
       exp[0..cnt-1] hold their exponents. */
    uint64_t primes[64];
    uint64_t exp[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exp, &cnt);

    /* Compute the radical (product of distinct primes). */
    seq_t rad = 1;
    for (size_t i = 0; i < cnt; ++i)
        rad *= (seq_t)primes[i];

    /* Test whether n is squarefree */
    bool squarefree = true;
    for (size_t i = 0; i < cnt; ++i)
        if (exp[i] > 1) { squarefree = false; break; }

    if (!squarefree)
        return rad;                           /* largest squarefree proper divisor */

    /* n is squarefree: remove the smallest prime factor */
    uint64_t smallest = primes[0];
    for (size_t i = 1; i < cnt; ++i)
        if (primes[i] < smallest) smallest = primes[i];

    return rad / (seq_t)smallest;              /* rad == n, so result < n */
}
