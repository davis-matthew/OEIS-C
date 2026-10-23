/*
 * A131259  a(2*n) = A000217(n), a(2*n+1) = -2*A000217(n).
 * https://oeis.org/A131259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(2*n) = n*(n+1)/2, a(2*n+1) = -2*n*(n+1)/2
 * Range:     i8 n <= 22, i16 n <= 362, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Compute k = floor(n/2) */
    idx_t k = n >> 1;

    /* Compute the triangular number T(k) = k*(k+1)/2
       Doing the division by 2 first avoids overflow for 64-bit types */
    seq_t kk = k;
    seq_t kp1 = k + 1;
    if (kk & 1)          /* kk is odd, divide kp1 */
        kp1 >>= 1;
    else
        kk >>= 1;        /* kp1 is odd, divide kk */

    seq_t tri = kk * kp1;

    /* Return tri for even n, and -2*tri for odd n */
    if (n & 1)  /* odd */
        return (seq_t)(-2) * tri;
    else
        return tri;
}
