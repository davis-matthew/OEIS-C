/*
 * A040202  Continued fraction for sqrt(217).
 * https://oeis.org/A040202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 14, a(n) for n>=1 is the (n-1)-th term in the period
 *            [1,2,1,2,1,1,9,4,9,1,1,2,1,2,1,28] for the simple continued
 *            fraction of sqrt(217)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)14;
    /* Period of length 16 starts from a(1) */
    static const seq_t period[16] = {1, 2, 1, 2, 1, 1,
                                     9, 4, 9, 1, 1, 2,
                                     1, 2, 1, 28};
    idx_t idx = (n - 1) & 15; /* modulo 16 (fast mod for power-of-2 length) */
    return period[idx];
}
