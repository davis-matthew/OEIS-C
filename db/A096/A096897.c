/*
 * A096897  Least semiperimeter of primitive Pythagorean triangles with even
 *          leg 4n.
 * https://oeis.org/A096897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   gcd64_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd64_local.h"
/* This computes the least semiperimeter of a primitive Pythagorean
   triangle whose even leg is 4n.  For a given n, all factor pairs
   (m,k) with m·k = 2n, gcd(m,k)=1, m>k, and m and k of opposite
   parity are examined; the minimal value of the semiperimeter
   s = m(m+k) is returned. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* offset starts at 1; 0 is unused */

    idx_t two_n = n * 2;                      /* 2n fits in idx_t for all test values */

    seq_t best = 0;                           /* 0 = unset */

    /* enumerate divisors d of 2n; d will be k, and m = 2n/d */
    for (idx_t d = 1; d <= two_n; ++d) {
        if (two_n % d != 0) continue;

        idx_t k = d;
        idx_t m = two_n / d;

        if (m <= k) continue;                /* need m>k */

        if (gcd64_local(m, k) != 1) continue;/* primitive condition */
        if (((m ^ k) & 1) == 0) continue;   /* opposite parity */

        seq_t s = (seq_t)m * (m + k);
        if (best == 0 || s < best) best = s;
    }

    /* There is always at least one valid pair: (m=2n, k=1) */
    return best;
}
