/*
 * A066625  Concatenation of factorials in increasing order up to the n-th and
 *          then in decreasing order.
 * https://oeis.org/A066625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = concat(1! 2! … n! (n-1)! … 2! 1!)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset starts at 1, guard anyway */

    seq_t result = 0;
    seq_t fact   = 1;
    idx_t digits = 1;                   /* number of decimal digits of current fact */

    /* forward part: 1! … n! */
    for (idx_t i = 1; i <= n; ++i) {
        fact *= (seq_t)i;               /* i! */

        /* adjust digit counter */
        seq_t pow10d = pow10(digits);
        while (fact >= pow10d) {        /* digits increased */
            ++digits;
            pow10d = pow10(digits);
        }

        result = result * pow10(digits) + fact;
    }

    /* reverse part: (n-1)! … 1! */
    for (idx_t i = n; i > 1; --i) {
        fact /= (seq_t)i;               /* (i-1)! from i! */

        /* adjust digit counter if it decreased */
        if (digits > 1) {
            seq_t pow10d_minus = pow10(digits - 1);
            while (fact < pow10d_minus) {
                --digits;
                if (digits <= 1) break;
                pow10d_minus = pow10(digits - 1);
            }
        }

        /* adjust digit counter if it increased (unlikely in reverse) */
        seq_t pow10d = pow10(digits);
        while (fact >= pow10d) {
            ++digits;
            pow10d = pow10(digits);
        }

        result = result * pow10(digits) + fact;
    }

    return result;
}
