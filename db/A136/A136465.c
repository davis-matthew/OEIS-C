/*
 * A136465  Row 0 of square array A136462: a(n) = C(2^(n-1), n) for n>=0.
 * https://oeis.org/A136465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2^(n-1), n) for n >= 0
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"   /* pow2(idx_t) -> seq_t */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* N = 2^(n-1) */
    seq_t N = pow2(n - 1);

    seq_t result = 1;
    /* Use the multiplicative binomial formula:
       C(N, k) = Π_{i=1..k} (N - k + i)/i
       Here k = n */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t offset   = (seq_t)(n - i);   /* n - i   */
        seq_t numerator = N - offset;      /* N - (n-i) = N - k + i  */
        seq_t denom     = (seq_t)i;        /* i */
        result = (result * numerator) / denom;
    }

    return result;
}
