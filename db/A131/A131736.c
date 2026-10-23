/*
 * A131736  Period 6: repeat [0, 0, 1, -1, -1, 1].
 * https://oeis.org/A131736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = pattern[n % 6] where pattern = {0, 0, 1, -1, -1, 1}
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Period 6: repeat [0, 0, 1, -1, -1, 1]. Offset 0. */

seq_t a(idx_t n)
{
    /* Static lookup table for one period; works for any seq_t type */
    static const seq_t pattern[6] = {
        (seq_t)0,
        (seq_t)0,
        (seq_t)1,
        (seq_t)-1,
        (seq_t)-1,
        (seq_t)1
    };
    /* n is unsigned idx_t; modulo 6 stays within range of idx_t */
    return pattern[n % 6];
}
