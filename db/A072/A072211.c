/*
 * A072211  a(n) = p-1 if n=p, p if n=p^e and e<>1, 1 otherwise; p a prime.
 * https://oeis.org/A072211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by prime factorisation a(n) = p-1 if n = p
 *            (prime) a(n) = p if n = p^e with e > 1 a(n) = 1 otherwise
 *            (offset 1: a(1) = 1)
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Handle the offset   */
    if (n == 0) return (seq_t)1;   /* value for indices below offset is irrelevant */

    /* n == 1 has value 1 */
    if (n == 1) return (seq_t)1;

    /* Factor n by trial division.  n fits in idx_t, which is a
       64‑bit unsigned integer in the build.  All arithmetic uses
       unsigned integers only, so it is safe from overflow for
       the intended test range (< 1e12). */
    idx_t temp = n;
    idx_t prime = 0;
    unsigned int exp = 0;
    unsigned int distinct = 0;

    /* Trial division up to sqrt(temp) */
    for (idx_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p == 0)
        {
            ++distinct;
            unsigned int e = 0;
            while (temp % p == 0) { temp /= p; ++e; }

            if (distinct == 1)          /* first distinct prime */
            {
                prime = p;
                exp   = e;
            }
            else                        /* found a second distinct prime */
            {
                /* No need to continue, result will be 1 */
                return (seq_t)1;
            }
        }
    }

    /* If a prime factor remains larger than sqrt(original n) */
    if (temp > 1)
    {
        ++distinct;
        if (distinct == 1)          /* n itself is prime */
        {
            prime = temp;
            exp   = 1;
        }
        else                        /* second distinct prime found */
        {
            return (seq_t)1;
        }
    }

    /* Determine the value according to the definition */
    if (distinct == 1)
    {
        if (exp == 1)               /* n is a prime */
            return (seq_t)(prime - 1);
        else                        /* n is a prime power with exponent > 1 */
            return (seq_t)(prime);
    }

    /* All other cases */
    return (seq_t)1;
}
