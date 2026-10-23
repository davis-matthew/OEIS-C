/*
 * A115360  Period 6: repeat [1,-1,1,0,0,0].
 * https://oeis.org/A115360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) repeats the 6‑term pattern [1,‑1,1,0,0,0]
 * Range:     i8 n <= 80, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pattern[6] = {1, -1, 1, 0, 0, 0};
    return pattern[n % 6];
}
