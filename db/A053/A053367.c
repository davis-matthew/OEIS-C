/*
 * A053367  Partial sums of A050494.
 * https://oeis.org/A053367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+7,7)*(3*n+8)/8
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 48, u64 n <= 836, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Compute C(n+7,7) */
    seq_t b = binomial(n + 7, 7);

    /* Linear factor (3*n + 8) */
    seq_t t = (seq_t)n * 3 + 8;

    /* Reduce division by 8 by cancelling common factors */
    seq_t denom = 8;
    seq_t g = gcd(b, denom);
    b /= g;
    denom /= g;

    g = gcd(t, denom);
    t /= g;
    denom /= g;   /* denom should now be 1 */

    /* Final result */
    return b * t;
}
