/*
 * A240349  Inverse of 59th cyclotomic polynomial.
 * https://oeis.org/A240349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n mod 59 == 0, -1 if n mod 59 == 1, else 0
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t PERIOD = 59;
    idx_t r = n % PERIOD;
    if (r == 0)      return (seq_t)1;
    if (r == 1)      return (seq_t)-1;
    return (seq_t)0;
}
