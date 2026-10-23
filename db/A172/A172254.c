/*
 * A172254  1 followed by period 4: repeat [-1, 3, -2, -1].
 * https://oeis.org/A172254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, less, easy
 * Formula:   a(1) = 1; for n>=2, a(n) repeats the period [-1, 3, -2, -1]
 *            (period 4)
 * Range:     i8 n <= 77, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* safety for invalid indices */
    if (n == 1) return 1;
    static const seq_t pattern[4] = { -1, 3, -2, -1 };
    return pattern[(n - 2) % 4];
}
