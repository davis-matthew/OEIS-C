/*
 * A158678  Period 18: repeat [0, 0, 0, 3, 0, 0, 0, 3, 0, -3, 0, 3, 0, -3, 0,
 *          0, 0, -3].
 * https://oeis.org/A158678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = periodic with period 18:
 *            [0,0,0,3,0,0,0,3,0,-3,0,3,0,-3,0,0,0,-3]
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    static const seq_t table[18] = {
        0, 0, 0, 3, 0, 0, 0, 3, 0, -3, 0, 3, 0, -3, 0, 0, 0, -3
    };
    return table[(n - 1) % 18];
}
