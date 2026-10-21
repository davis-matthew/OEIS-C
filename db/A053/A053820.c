/*
 * A053820  a(n) = Sum_{k=1..n, gcd(n,k) = 1} k^4.
 * https://oeis.org/A053820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n, gcd(n,k)=1} k^4
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 120, u64 n <= 9850,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Direct enumeration: for each k in [1..n] test gcd(k,n)=1 and add k^4. */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for offset, but prevents UB */

    seq_t sum = 0;
    seq_t sn = (seq_t)n;

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t sk = (seq_t)k;
        if (gcd(sk, sn) == 1)
        {
            /* compute k^4 safely: (k^2)^2 */
            seq_t k2 = sk * sk;
            seq_t k4 = k2 * k2;
            sum += k4;
        }
    }
    return sum;
}
