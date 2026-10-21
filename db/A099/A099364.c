/*
 * A099364  An inverse Chebyshev transform of (1-x)^2.
 * https://oeis.org/A099364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(2n) = Catalan(n+1), a(2n+1) = -2 * Catalan(n+1)
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {
        return catalan(n / 2 + 1);
    } else {
        idx_t half = (n - 1) / 2;
        seq_t c = catalan(half + 1);
        return -(c * 2);
    }
}
