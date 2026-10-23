/*
 * A199084  a(n) = Sum_{k=1..n} (-1)^(k+1) gcd(k,n).
 * https://oeis.org/A199084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = sum_{k = 1..n} (-1)^(k+1) * gcd(k, n)
 * Range:     i8 n <= 47, i16 n <= 2879, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses the provided gcd routine from
 * the utils library.  It is O(n) per call and works for
 * any seq_t that supports the basic arithmetic operators.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t nn  = (seq_t)n;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t g = gcd((seq_t)k, nn);
        if ((k & 1) != 0)          /* k is odd */
            sum += g;
        else                       /* k is even */
            sum -= g;
    }
    return sum;
}
