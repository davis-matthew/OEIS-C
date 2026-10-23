/*
 * A132953  a(n) is the sum of the isolated totatives of n.
 * https://oeis.org/A132953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of isolated totatives of n
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 9884, u64 n <= 9884,
 *            u128 n <= 9884, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* An isolated totative k of n satisfies:
   gcd(k, n) = 1,
   gcd(k-1, n) > 1,
   gcd(k+1, n) > 1
   (k ranges from 1 to n). */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        /* k must be coprime to n */
        if (gcd((seq_t)k, (seq_t)n) == 1)
        {
            /* check neighbors not coprime */
            if (gcd((seq_t)(k - 1), (seq_t)n) != 1 &&
                gcd((seq_t)(k + 1), (seq_t)n) != 1)
            {
                sum += (seq_t)k;
            }
        }
    }
    return sum;
}
