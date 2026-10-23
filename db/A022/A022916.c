/*
 * A022916  Multinomial coefficient n!/([n/3]![(n+1)/3]![(n+2)/3]!).
 * https://oeis.org/A022916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n! / ([n/3]![(n+1)/3]![(n+2)/3]!) = C(n, floor(n/3)) *
 *            C(n - floor(n/3), floor((n+1)/3))
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 44, u128 n <= 84,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t k = n / 3;
    idx_t l = (n + 1) / 3;   /* floor((n+1)/3) */
    return binomial(n, k) * binomial(n - k, l);
}
