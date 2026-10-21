/*
 * A080275  a(n)=(-1)^n(1 - (1/12)n(n + 1)(12 - n + n^2)).
 * https://oeis.org/A080275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (-1)^n * (1 - (1/12) * n*(n + 1)*(12 - n + n^2))
 * Range:     i8 n <= 2, i16 n <= 13, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* convert index to seq_t and compute the expression step by step */
    seq_t term = (seq_t)n * ((seq_t)n + 1) *
                 ((((seq_t)n * (seq_t)n) - (seq_t)n + 12));
    term /= 12;                       /* exact division */
    /* result: even n -> 1 - term, odd n -> term - 1 */
    return (n & 1) ? (term - 1) : (1 - term);
}
