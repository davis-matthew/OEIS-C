/*
 * A213824  Antidiagonal sums of the convolution array A213822.
 * https://oeis.org/A213824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(2 + 3*n + 3*n^2)/4
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 274, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t a1 = tn;
    seq_t b1 = tn + 1;
    seq_t c1 = (seq_t)2 + (seq_t)3 * tn + (seq_t)3 * tn * tn;

    /* first division by 2 */
    if ((a1 & 1) == 0) {
        a1 /= 2;
    } else if ((b1 & 1) == 0) {
        b1 /= 2;
    } else {
        c1 /= 2;
    }

    /* second division by 2 */
    if ((a1 & 1) == 0) {
        a1 /= 2;
    } else if ((b1 & 1) == 0) {
        b1 /= 2;
    } else {
        c1 /= 2;
    }

    return a1 * b1 * c1;
}
