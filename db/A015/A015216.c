/*
 * A015216  Inverse of 1207th cyclotomic polynomial.
 * https://oeis.org/A015216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Inverse of the 1207th cyclotomic polynomial.
   The coefficients of the series expansion of 1/Φ_1207(x) are
   periodic with period 1207 and are given by
      a(n) =  1,  if 0 ≤ (n mod 1207) < 17
            =  0,  if 17 ≤ (n mod 1207) < 71
            = -1, if 71 ≤ (n mod 1207) < 88
            =  0,  otherwise
*/

seq_t a(idx_t n)
{
    const idx_t PERIOD = 1207;
    idx_t r = n % PERIOD;          /* 0 .. 1206 */

    if (r < 17)          return (seq_t)1;
    if (r < 71)          return (seq_t)0;
    if (r < 88)          return (seq_t)(-1);
    return (seq_t)0;
}
