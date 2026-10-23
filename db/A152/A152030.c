/*
 * A152030  a(n)=n^6-n^5-n^4-n^3-n^2-n.
 * https://oeis.org/A152030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n-1)^6-(n-1)^5-(n-1)^4-(n-1)^3-(n-1)^2-(n-1)
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 28, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t m  = (seq_t)n - 1;      // shift index to match formula
    seq_t m2 = m * m;             // (n-1)^2
    seq_t m3 = m2 * m;            // (n-1)^3
    seq_t m4 = m3 * m;            // (n-1)^4
    seq_t m5 = m4 * m;            // (n-1)^5
    seq_t m6 = m5 * m;            // (n-1)^6
    return m6 - m5 - m4 - m3 - m2 - m;
}
