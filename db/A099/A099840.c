/*
 * A099840  Expansion of (1-6*x)/(1-20*x^2).
 * https://oeis.org/A099840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0) = 1, a(1) = -6, a(n) = 20 * a(n-2) for n >= 2
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 14, i64 n <= 28, i128 n <= 58,
 *            big any n
 * Helpers:   pow20
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow20.h"

seq_t a(idx_t n) {
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)-6;
    if ((n & 1u) == 0u) {                /* even n */
        return pow20(n >> 1);
    } else {                              /* odd n */
        return (seq_t)-6 * pow20((n - 1u) >> 1);
    }
}
