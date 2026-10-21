/*
 * A216844  4k^2-8k+2 interleaved with 4k^2-4k+2 for k>=0.
 * https://oeis.org/A216844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1/2)*(2*n*(n-4)-3*(-1)^n+7)
 * Range:     i8 n <= 10, i16 n <= 51, i32 n <= 51, i64 n <= 51, i128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;                      /* convert index to seq_t */
    seq_t sign = (n & 1) ? (seq_t)-1 : (seq_t)1;   /* (-1)^n */

    /* compute 2*n*(n-4) */
    seq_t term = nn * (nn - (seq_t)4);
    term = term * (seq_t)2;

    /* add -3*(-1)^n + 7 */
    term += (-3 * sign) + (seq_t)7;

    /* divide by 2 (exact division) */
    return term / (seq_t)2;
}
