/*
 * A079553  a(n) = floor( d(n^2) / d(n) ), where d() = A000005.
 * https://oeis.org/A079553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(d(n^2)/d(n))
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where d(m) is the number of positive divisors of m. */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t dn   = 1;   /* d(n)   */
    seq_t dn2  = 1;   /* d(n²)  */

    idx_t m = n;
    for (idx_t p = 2; (unsigned __int128)p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            unsigned exp = 0;
            while (m % p == 0)
            {
                m /= p;
                ++exp;
            }
            dn  *= (seq_t)(exp + 1);
            dn2 *= (seq_t)(2 * exp + 1);
        }
    }
    if (m > 1)          /* remaining prime factor */
    {
        dn  *= 2;         /* exp = 1 → exp+1 = 2 */
        dn2 *= 3;         /* 2*1+1 = 3 */
    }

    return dn2 / dn;    /* integer division gives floor for positive numbers */
}
