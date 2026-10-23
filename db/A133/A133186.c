/*
 * A133186  Period 4: repeat [1, 2, 1, -4].
 * https://oeis.org/A133186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = cycle[n mod 4] where cycle = [1, 2, 1, -4] with offset 0
 * Range:     i8 n <= 75, i16 n <= 75, i32 n <= 75, i64 n <= 75, i128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence repeats every 4 terms: 1, 2, 1, -4. */
    static const seq_t cycle[4] = { (seq_t)1, (seq_t)2, (seq_t)1, (seq_t)-4 };
    return cycle[n & 3];
}
