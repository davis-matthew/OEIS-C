/*
 * A132954  Period 6: repeat [1, 2, 4, -1, -2, -4].
 * https://oeis.org/A132954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-3) for n > 2 with a(0)=1, a(1)=2, a(2)=4 (period 6
 *            pattern: [1, 2, 4, -1, -2, -4])
 * Range:     i8 n <= 73, i16 n <= 73, i32 n <= 73, i64 n <= 73, i128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 6‑term repeating pattern */
    static const seq_t pattern[6] = {
        (seq_t)1, (seq_t)2, (seq_t)4,
        (seq_t)-1, (seq_t)-2, (seq_t)-4
    };
    return pattern[n % 6];
}
