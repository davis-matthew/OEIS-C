/*
 * A015228  Inverse of 1219th cyclotomic polynomial.
 * https://oeis.org/A015228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 for 0 ≤ n mod 1219 ≤ 22; * a(n) = 0 for 23 ≤ n mod 1219
 *            ≤ 52; * a(n) = –1 for 53 ≤ n mod 1219 ≤ 75; * a(n) = 0 otherwise
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t period = 1219;
    idx_t r = n % period;

    if (r <= 22)            return (seq_t)1;
    if (r <= 52)            return (seq_t)0;
    if (r <= 75)            return (seq_t)-1;
    return (seq_t)0;
}
