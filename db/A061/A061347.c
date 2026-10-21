/*
 * A061347  Period 3: repeat [1, 1, -2].
 * https://oeis.org/A061347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = 1 if n mod 3 == 1 or 2, a(n) = -2 if n mod 3 == 0 (offset
 *            = 1)
 * Range:     i8 n <= 108, i16 n <= 108, i32 n <= 108, i64 n <= 108,
 *            i128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 3;
    if (r == 0)
        return (seq_t)-2;
    return (seq_t)1;
}
