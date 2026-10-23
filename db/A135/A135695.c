/*
 * A135695  Period 6: repeat [-1, -1, -2, -2, 3, 3].
 * https://oeis.org/A135695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-6) for n>5, with initial values a(0) = -1, a(1) = -1,
 *            a(2) = -2, a(3) = -2, a(4) = 3, a(5) = 3 Thus the sequence
 *            repeats the pattern [-1, -1, -2, -2, 3, 3]
 * Range:     i8 n <= 78, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pattern[6] = {
        (seq_t)-1, (seq_t)-1,
        (seq_t)-2, (seq_t)-2,
        (seq_t)3,  (seq_t)3
    };
    return pattern[n % 6];
}
