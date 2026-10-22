/*
 * A003687  a(n+1) = a(n)-a(1)a(2)...a(n-1), if n>0. a(0)=1, a(1)=2.
 * https://oeis.org/A003687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1, a(1) = 2, a(2) = 1; a(2k+1) = -1 for k >= 1; a(2k) =
 *            -t(k) for k >= 2, where t(2) = 3, t(k+1) = t(k)^2 - t(k) + 1
 *            (Sylvester's sequence). This follows from a(n+1) = a(n) -
 *            a(1)a(2)...a(n-1)
 * Range:     i8 n <= 9, i16 n <= 11, i32 n <= 13, i64 n <= 15, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)1;
    if (n % 2 == 1) return (seq_t)(-1);

    idx_t k = n / 2;
    seq_t t = 3;
    for (idx_t j = 2; j < k; j++) {
        t = t * (t - 1) + 1;
    }
    return -t;
}
