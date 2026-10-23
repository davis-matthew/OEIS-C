/*
 * A048250  Sum of the squarefree divisors of n.
 * https://oeis.org/A048250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, mult
 * Formula:   a(n) = product_{p | n, p prime} (p+1) (sum of squarefree
 *            divisors of n)
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    uint64_t m = (uint64_t)n;
    seq_t  result = (seq_t)1;

    for (uint64_t p = 2; p <= m / p; ++p)
    {
        if (m % p == 0)
        {
            result = result * ((seq_t)p + 1);
            while (m % p == 0)
                m /= p;
        }
    }

    if (m > 1)   /* remaining prime factor */
        result = result * ((seq_t)m + 1);

    return result;
}
