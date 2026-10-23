/*
 * A131379  Period 4: repeat [1, 0, -1, 1].
 * https://oeis.org/A131379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = [1, 0, -1, 1] repeated with period 4, a(0)=1
 * Range:     i8 n <= 79, i16 n <= 79, i32 n <= 79, i64 n <= 79, i128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pattern[4] = {1, 0, -1, 1};
    return pattern[n & 3];
}
