/*
 * A214719  Least m>0 such that 6^n-m and n-m are relatively prime.
 * https://oeis.org/A214719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least m>0 such that gcd(6^n - m, n - m) = 1
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    for (idx_t m = 1; ; ++m)
    {
        /* compute |n - m|, using unsigned arithmetic safely */
        idx_t diff64 = (n > m) ? (n - m) : (m - n);
        if (diff64 == 0)          /* m == n --> n - m == 0, gcd cannot be 1 */
            continue;

        seq_t diff = (seq_t)diff64;

        /* compute (6^n - m) mod diff  */
        seq_t t    = mod_pow((seq_t)6, n, diff);          /* 6^n mod diff   */
        seq_t mmod = (seq_t)(m % diff64);                 /* m mod diff     */

        seq_t rem;
        if (t >= mmod)
            rem = t - mmod;                     /* t - mmod (may be 0) */
        else
            rem = t + diff - mmod;              /* (t - mmod) mod diff */

        /* now gcd(rem, diff) == gcd(6^n - m, |n - m|) */
        if (gcd(rem, diff) == (seq_t)1)
            return (seq_t)m;
    }
}
