/*
 * A015046  Inverse of 1037th cyclotomic polynomial.
 * https://oeis.org/A015046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n mod 1037 <= 16; * a(n) = -1 if 61 <= n mod 1037 <=
 *            77; * otherwise a(n) = 0
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t period = 1037;
    idx_t r = n % period;

    if (r <= 16) {
        return (seq_t)1;
    }
    if (r >= 61 && r <= 77) {
        return (seq_t)-1;
    }
    return (seq_t)0;
}
