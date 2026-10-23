/*
 * A133936  Number of times prime powers occur in the columns of tables
 *          A133232 and A133233.
 * https://oeis.org/A133936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(p-1) if n is a prime power p^k; otherwise 0
 * Range:     u8 n <= 16, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    uint64_t nn = (uint64_t)n;
    uint64_t spf = 0;

    /* find the smallest prime factor (spf) of nn */
    for (uint64_t i = 2; i <= nn / i; ++i)
    {
        if (nn % i == 0)
        {
            spf = i;
            break;
        }
    }

    if (spf == 0)          /* nn is prime */
    {
        spf = nn;
    }
    else
    {
        /* check if nn is a power of spf */
        uint64_t tmp = nn;
        while (tmp % spf == 0)
            tmp /= spf;
        if (tmp != 1)
            return 0;      /* not a prime power */
    }

    /* compute n*(spf-1) in seq_t */
    return (seq_t)nn * ((seq_t)(spf - 1));
}
