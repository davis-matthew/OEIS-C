/*
 * A208881  Number of words either empty or beginning with the first letter of
 *          the ternary alphabet, where each letter of the alphabet occurs n
 *          times.
 * https://oeis.org/A208881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = (3*n)!/(3 * n!^3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 28,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 631 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0u) {           /* a(0) = 1 by definition */
        return 1;
    }

    /* binom(3n,n) * binom(2n,n) / 3 */
    seq_t t1 = binomial(3 * n, n);    /* (3n)!/(n!(2n)!) */
    seq_t t2 = binomial(2 * n, n);    /* (2n)!/(n!n!) */

    /* exactly one of t1 or t2 is divisible by 3 */
    if ((t1 % 3) == 0) {
        t1 /= 3;
    } else {
        t2 /= 3;
    }

    return t1 * t2;
}
