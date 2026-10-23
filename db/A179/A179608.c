/*
 * A179608  a(n) = (7 + (-1)^n + 6*n)*2^(n-3).
 * https://oeis.org/A179608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (7 + (-1)^n + 6*n)*2^(n-3) (with a(0)=1)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* Compute the factor 7 + (-1)^n + 6*n.
       For odd n: (-1)^n = -1  → factor = 6 + 6*n
       For even n: (-1)^n =  1  → factor = 8 + 6*n */
    seq_t factor;
    if (n & 1U) {
        factor = (seq_t)(6 + 6 * n);
    } else {
        factor = (seq_t)(8 + 6 * n);
    }

    /* Multiply by 2^(n-3). Handle n < 3 with division. */
    if (n < 3) {
        seq_t denom = pow2(3 - n);            /* 2, 4, or 8 */
        return factor / denom;
    } else {
        return factor * pow2(n - 3);
    }
}
