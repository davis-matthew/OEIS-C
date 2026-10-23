/*
 * A193885  a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) - a(n-4), n>=4; a(0) = 1, a(1)
 *          = 1, a(2) = 2, a(3) = 3.
 * https://oeis.org/A193885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=2, a(3)=3;
 *            a(n)=3*a(n-1)-3*a(n-2)+a(n-3)-a(n-4) for n>=4
 * Range:     i8 n <= 10, i16 n <= 22, i32 n <= 44, i64 n <= 89, i128 n <= 174,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the first four indices directly */
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 3;

    /* keep only the last four values to avoid dynamic allocation */
    seq_t v0 = 1;    /* a(n-4) */
    seq_t v1 = 1;    /* a(n-3) */
    seq_t v2 = 2;    /* a(n-2) */
    seq_t v3 = 3;    /* a(n-1) */
    seq_t v4;        /* a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        v4 = 3 * v3 - 3 * v2 + v1 - v0;
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = v4;
    }
    return v4;
}
