/*
 * A131561  Period 3: repeat [1, 1, -1].
 * https://oeis.org/A131561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n % 3 == 2) ? -1 : 1; (periodic pattern [1, 1, -1])
 * Range:     i8 n <= 109, i16 n <= 109, i32 n <= 109, i64 n <= 109,
 *            i128 n <= 109, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 3;
    return (r == 2) ? (seq_t)-1 : (seq_t)1;
}
