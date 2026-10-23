/*
 * A192382  Coefficient of x in the reduction by x^2 -> x+2 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4^(n-1) - (-2)^(n-1)) / 3 (coefficient of x in the
 *            reduction of p(n-1, x) by x^2 -> x+2)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* safety, sequence defined for n >= 1 */

    /* n == 1 gives 0 directly */
    if (n == 1) return 0;

    idx_t k = n - 1;              /* exponent for powers */

    seq_t pow4 = 1;               /* 4^k */
    seq_t pow2 = 1;               /* 2^k */
    for (idx_t i = 0; i < k; ++i) {
        pow4 *= 4;
        pow2 *= 2;
    }

    seq_t diff;
    if ((k & 1) == 0) {
        /* (-2)^k = +2^k */
        diff = pow4 - pow2;
    } else {
        /* (-2)^k = -2^k */
        diff = pow4 + pow2;
    }

    return diff / 3;
}
