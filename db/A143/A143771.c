/*
 * A143771  a(n) = gcd(k + n/k), where k is over all divisors of n.
 * https://oeis.org/A143771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd( k + n/k for all divisors k of n )
 * Range:     u8 n <= 256, u16 n <= 9383, u32 n <= 9383, u64 n <= 9383,
 *            u128 n <= 9383, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Computes OEIS A143771: for each divisor k of n,
   compute k + n/k, then return the greatest common divisor of
   all those values. */
seq_t a(idx_t n)
{
    if (n == 0) {          /* n=0 is outside the defined range */
        return 0;
    }

    seq_t g = 0;           /* start with 0 so the first value becomes g */

    /* Iterate over divisors up to sqrt(n) only; each divisor i
       generates the same value as its pair n/i, so we need
       only process one of each pair. */
    for (idx_t i = 1; i <= n / i; ++i) {
        if ((n % i) == 0) {
            idx_t d = i;
            seq_t val = (seq_t)d + (seq_t)(n / d);
            if (g == 0) {
                g = val;
            } else {
                g = gcd(g, val);
            }
        }
    }

    return g;
}
