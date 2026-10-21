/*
 * A084219  Inverse binomial transform of A053088.
 * https://oeis.org/A084219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (4 - 3*n*(-2)^(n-1) + 5*(-2)^n)/9
 * Range:     i8 n <= 3, i16 n <= 10, i32 n <= 25, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   pow_neg_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_neg_two.h"

/* Compute (-2)^k in seq_t */

seq_t a(idx_t n)
{
    seq_t pow_n   = pow_neg_two(n);                           /* (-2)^n   */
    seq_t pow_n1  = n > 0 ? pow_neg_two(n - 1) : 0;           /* (-2)^(n-1) or 0 */
    seq_t num = 4
                + 5 * pow_n
                - 3 * (seq_t)n * pow_n1;                    /* numerator   */
    return num / 9;                                          /* divide exactly */
}
