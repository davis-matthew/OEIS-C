/*
 * A015156  Inverse of 1147th cyclotomic polynomial.
 * https://oeis.org/A015156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (⌊n/1147⌋+1) – (if n≥31 then ⌊(n−31)/1147⌋+1 else 0) –
 *            (if n≥37 then ⌊(n−37)/1147⌋+1 else 0) + (if n≥68 then
 *            ⌊(n−68)/1147⌋+1 else 0)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* main term */
    seq_t h0 = (seq_t)(n / 1147U) + 1;

    /* shifted terms, only if the shift does not make the index negative */
    seq_t h31 = 0, h37 = 0, h68 = 0;
    if (n >= 31U)
        h31 = (seq_t)((n - 31U) / 1147U) + 1;
    if (n >= 37U)
        h37 = (seq_t)((n - 37U) / 1147U) + 1;
    if (n >= 68U)
        h68 = (seq_t)((n - 68U) / 1147U) + 1;

    return h0 - h31 - h37 + h68;
}
