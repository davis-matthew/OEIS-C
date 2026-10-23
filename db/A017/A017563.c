/*
 * A017563  a(n) = (12*n + 3)^7.
 * https://oeis.org/A017563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 3)^7
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 46, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute t = 12*n + 3 in seq_t */
    seq_t t = (seq_t)12 * (seq_t)n + (seq_t)3;

    /* Compute t^7 using a few multiplications */
    seq_t t2 = t * t;          /* t^2 */
    seq_t t3 = t2 * t;         /* t^3 */
    seq_t t6 = t3 * t3;        /* t^6 */
    seq_t t7 = t6 * t;         /* t^7 */

    return t7;
}
