/*
 * A024015  a(n) = 2^n-n^5.
 * https://oeis.org/A024015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2^n - n^5
 * Range:     i8 n <= 2, i16 n <= 8, i32 n <= 31, i64 n <= 63, i128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t pow5 = nn;          /* nn^1 */
    pow5 *= nn;               /* nn^2 */
    pow5 *= nn;               /* nn^3 */
    pow5 *= nn;               /* nn^4 */
    pow5 *= nn;               /* nn^5 */
    seq_t pow2n = pow2(n);
    return pow2n - pow5;
}
