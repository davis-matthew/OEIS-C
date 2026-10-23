/*
 * A134751  Hankel transform of expansion of (1/(1-x^2))c(x/(1-x^2)), where
 *          c(x) is the g.f. of A000108.
 * https://oeis.org/A134751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^floor((n+1)^2/3)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"   /* seq_t pow2(idx_t exp) */

seq_t a(idx_t n) {
    /* Compute the exponent e = floor((n+1)^2 / 3) */
    idx_t t = n + 1;
    idx_t e = (t * t) / 3;   /* integer division gives the floor */
    return pow2(e);
}
