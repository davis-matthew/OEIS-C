/*
 * A079578  Least number coprime to n and greater than n+1.
 * https://oeis.org/A079578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least integer m > n+1 such that gcd(m,n)=1
 * Range:     u8 n <= 251, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1; guard against n=0 */

    seq_t nn = (seq_t)n;             /* convert index to seq_t for gcd */
    seq_t m  = nn + (seq_t)2;        /* start searching at n+2 */

    for (;; ++m)                     /* increment until coprime found */
    {
        if (gcd(m, nn) == (seq_t)1)
            return m;
    }
}
