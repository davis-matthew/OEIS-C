/*
 * A136591  Column 1 of triangle A136590.
 * https://oeis.org/A136591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (n-1)! * (1 if n%3 != 0 else -2)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 21, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* sequence undefined for n<1, return 0 */
    seq_t fact = 1;
    for (idx_t i = 2; i < n; ++i) {
        fact = fact * (seq_t)i;
    }
    if (n % 3 == 0) {
        fact = fact * (seq_t)-2;
    }
    return fact;
}
