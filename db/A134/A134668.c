/*
 * A134668  Period 6: repeat [1, -1, 0, 0, -1, 1].
 * https://oeis.org/A134668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = [1, -1, 0, 0, -1, 1][n mod 6] (period 6)
 * Range:     i8 n <= 77, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pattern[6] = {
        (seq_t)1,  /* a(0) */
        (seq_t)-1, /* a(1) */
        (seq_t)0,  /* a(2) */
        (seq_t)0,  /* a(3) */
        (seq_t)-1, /* a(4) */
        (seq_t)1   /* a(5) */
    };
    return pattern[n % 6];
}
