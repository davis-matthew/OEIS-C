/*
 * A216178  Period 4: repeat [4, 1, 0, -3].
 * https://oeis.org/A216178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = {4, 1, 0, -3}[n mod 4] – period 4 sequence
 * Range:     i8 n <= 79, i16 n <= 79, i32 n <= 79, i64 n <= 79, i128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[4] = {4, 1, 0, -3};
    /* n ≥ 0; n & 3 gives n mod 4 safely in unsigned arithmetic */
    int idx = (int)(n & 3ULL);
    return period[idx];
}
