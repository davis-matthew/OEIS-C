/*
 * A040274  Continued fraction for sqrt(292).
 * https://oeis.org/A040274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = nth coefficient in the continued fraction expansion of
 *            sqrt(292)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* sqrt(292) = 2 * sqrt(73).  Its continued fraction starts with 17. */
    const int64_t D   = 292;   /* the number under the square root */
    const int64_t a0  = 17;    /* floor(sqrt(292)) */

    if (n == 0)
        return (seq_t)a0;

    int64_t m      = 0;
    int64_t d      = 1;
    int64_t a_curr = a0;
    for (idx_t i = 1; i <= n; ++i) {
        m      = d * a_curr - m;
        d      = (D - m * m) / d;
        a_curr = (a0 + m) / d;
    }
    return (seq_t)a_curr;
}
