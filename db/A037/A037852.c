/*
 * A037852  Number of normal subgroups of dihedral group with 2n elements.
 * https://oeis.org/A037852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1001, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037852:
 *  a(n) = d(n) + 2 + (-1)^n
 *  where d(n) is the number of positive divisors of n.
 *  For n even: a(n) = d(n) + 3
 *  For n odd : a(n) = d(n) + 1
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: compute the number of divisors of n.
 * Uses trial division up to sqrt(n).  For n up to 10^7 this is trivial,
 * and is well within the required 2‑second limit for all likely test cases. */
static seq_t divisor_count(idx_t n)
{
    seq_t result = 1;
    idx_t tmp = n;

    for (idx_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            seq_t exp = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++exp;
            }
            result *= (exp + 1);
        }
    }
    /* If tmp > 1, it is a prime factor with exponent 1 */
    if (tmp > 1)
        result *= 2;

    return result;
}

seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence */
    seq_t d = divisor_count(n);
    if (n & 1)   /* odd n */
        return d + 1;
    else         /* even n */
        return d + 3;
}
