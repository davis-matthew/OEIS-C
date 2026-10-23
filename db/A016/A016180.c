/*
 * A016180  Inverse of 2171st cyclotomic polynomial.
 * https://oeis.org/A016180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ⌊n/2171⌋+1 * − ⌊(n-13)/2171⌋ (if n≥13) * − ⌊(n-167)/2171⌋
 *            (if n≥167) * + ⌊(n-180)/2171⌋ (if n≥180) * the floor of a
 *            negative argument is treated as 0. * This implements the
 *            reciprocal of the 2171‑st cyclotomic polynomial
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const idx_t period = 2171;
    const idx_t d13     = 13;
    const idx_t d167    = 167;
    const idx_t d180    = 180;        /* 13 + 167 */

    seq_t term  = (seq_t)(n / period) + 1;
    seq_t sub13 = 0, sub167 = 0, add180 = 0;

    if (n >= d13)   sub13 = (seq_t)((n - d13) / period) + 1;
    if (n >= d167)  sub167 = (seq_t)((n - d167) / period) + 1;
    if (n >= d180)  add180 = (seq_t)((n - d180) / period) + 1;

    return term - sub13 - sub167 + add180;
}
