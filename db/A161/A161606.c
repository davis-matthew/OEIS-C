/*
 * A161606  a(n) = gcd(A008472(n), A001222(n)).
 * https://oeis.org/A161606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd( sum of distinct prime factors of n, * sum of
 *            multiplicities of prime factors of n )
 * Range:     u8 n <= 2929, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;          /* special case: n = 1 has no prime factors */

    uint64_t m = (uint64_t)n;            /* factorization works on 64‑bit values */
    seq_t sum_distinct = 0;
    seq_t sum_exp     = 0;

    /* factorize m by trial division */
    uint64_t p = 2;
    while (p <= m / p)       /* avoid overflow in p*p */
    {
        if (m % p == 0)
        {
            /* p is a prime factor with multiplicity e */
            int e = 0;
            do
            {
                m /= p;
                ++e;
            } while (m % p == 0);

            sum_distinct += (seq_t)p;
            sum_exp     += (seq_t)e;
        }
        p = (p == 2) ? 3 : p + 2; /* after checking 2, test only odd p */
    }

    /* if m > 1 now, m itself is a remaining prime factor */
    if (m > 1)
    {
        sum_distinct += (seq_t)m; /* exponent is 1 */
        sum_exp     += (seq_t)1;
    }

    return gcd(sum_distinct, sum_exp);
}
