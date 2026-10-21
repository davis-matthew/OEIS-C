/*
 * A210628  Expansion of (-1 + 2*x + sqrt( 1 - 4*x^2)) / (2*x) in powers of x.
 * https://oeis.org/A210628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1 a(n) = 0 if n > 0 and n is even a(n) =
 *            -Catalan((n-1)/2) if n is odd (n > 0) where Catalan(k) = (2k)! /
 *            (k! (k+1)!) The helper function `catalan(idx_t k)` is provided
 *            in "../utils/catalan.h"
 * Range:     i8 n <= 10, i16 n <= 18, i32 n <= 34, i64 n <= 72, i128 n <= 138,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    /* even index (n > 0) → value 0 */
    if ((n & 1ULL) == 0) {
        return (seq_t)0;
    }
    /* odd index: compute k = (n-1)/2 and return -Catalan(k) */
    idx_t k = (n - 1) >> 1;
    seq_t c = catalan(k);
    return -c;
}
