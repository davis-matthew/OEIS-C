/*
 * A008438  Sum of divisors of 2*n + 1.
 * https://oeis.org/A008438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = sum of divisors of (2*n + 1) (OEIS A008438)
 * Range:     u8 n <= 81, u16 n <= 9484, u32 n <= 9484, u64 n <= 9484,
 *            u128 n <= 9484, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Sum of divisors of an odd number 2*n+1 */
seq_t a(idx_t n)
{
    /* Convert index to the target odd integer */
    uint64_t m = 2ull * (uint64_t)n + 1ull;   /* 2*n + 1 fits in 64 bits for n up to 10 000 */
    seq_t result = 1;                         /* multiplicative identity for sigma */

    uint64_t temp = m;

    /* factor m by trial division (m is odd, but we include 2 for generality) */
    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            seq_t term   = 1;   /* sum for this prime factor */
            seq_t pow    = 1;   /* current p^k */

            while (temp % p == 0)
            {
                temp /= p;
                pow *= p;
                term += pow;
            }

            result *= term;     /* accumulate multiplicatively */
        }
    }

    /* If a prime factor greater than sqrt(m) remains, it appears only once */
    if (temp > 1)
    {
        seq_t one_plus = (seq_t)temp + 1;
        result *= one_plus;
    }

    return result;
}
