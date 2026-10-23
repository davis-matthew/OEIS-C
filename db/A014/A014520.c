/*
 * A014520  Inverse of 511th cyclotomic polynomial.
 * https://oeis.org/A014520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n mod 511 ∈ [0,6]; * a(n) = -1 if n mod 511 ∈
 *            [73,79]; * otherwise a(n) = 0. * The sequence has period 511
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t period = 511;
    idx_t r = n % period;
    if (r <= 6)      return (seq_t)1;
    if (r <= 72)     return (seq_t)0;
    if (r <= 79)     return (seq_t)(-1);
    return (seq_t)0;
}
