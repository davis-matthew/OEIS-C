/*
 * A135448  Period 5: repeat [1, 5, 3, 4, -2].
 * https://oeis.org/A135448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = a(n-5) for n >= 5 a(0) = 1, a(1) = 5, a(2) = 3, a(3) = 4,
 *            a(4) = -2
 * Range:     i8 n <= 95, i16 n <= 95, i32 n <= 95, i64 n <= 95, i128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic values repeating every 5 indices */
    static const seq_t vals[5] = {1, 5, 3, 4, (seq_t)-2};
    return vals[n % 5];
}
