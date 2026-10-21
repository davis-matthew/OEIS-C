/*
 * A062553  Number of Abelian subgroups of the dihedral group with 2n
 *          elements.
 * https://oeis.org/A062553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + τ(n) if n is odd, (3/2)*n + τ(n) if n is even, where
 *            τ(n) is the number of divisors of n
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t divisor_count(idx_t n)
{
    /* Count divisors of n (n ≥ 1).  The result fits in seq_t for all
       values used in the test set. */
    seq_t count = 1;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            idx_t exp = 0;
            do
            {
                m /= p;
                ++exp;
            } while (m % p == 0);
            count *= (seq_t)(exp + 1);
        }
    }
    if (m > 1)          /* a remaining prime factor */
        count *= 2;
    return count;
}

seq_t a(idx_t n)
{
    /* No need to handle n < 1; the OEIS offset is 1. */
    seq_t tau = divisor_count(n);

    if (n & 1)            /* odd n */
        return (seq_t)n + tau;
    else                  /* even n */
    {
        /* compute floor(3*n/2) without overflowing 3*n */
        seq_t half = (seq_t)(n >> 1);            /* n/2 */
        seq_t three_over_two = half * 3 + (seq_t)(n & 1);
        return three_over_two + tau;
    }
}
