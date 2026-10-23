/*
 * A176563  Period 4: repeat [1, -3, 1, 1].
 * https://oeis.org/A176563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = [1, -3, 1, 1][n mod 4] (period 4)
 * Range:     i8 n <= 92, i16 n <= 92, i32 n <= 92, i64 n <= 92, i128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t periodic[4] = { 1, -3, 1, 1 };

seq_t a(idx_t n)
{
    unsigned int i = (unsigned int)(n & 3ULL);
    return periodic[i];
}
