/*
 * A076388  a(n) = minimum of y-x such that x <= y, x*y = n and gcd(x,y)=1.
 * https://oeis.org/A076388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = minimum of y - x where x <= y, x*y = n and gcd(x,y) = 1
 *            a(1) = 0
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* handle the trivial case */
    if (n <= 1) return 0;

    seq_t minDiff = (seq_t)(n - 1);      /* start with the pair (1,n) */

    /* iterate over all divisor pairs (d, n/d) with d <= n/d */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0) continue;       /* skip non‑divisors */

        idx_t other = n / d;
        if (gcd((seq_t)d, (seq_t)other) == 1) {
            seq_t diff = (seq_t)other - (seq_t)d;
            if (diff < minDiff) minDiff = diff;
        }
    }

    return minDiff;
}
