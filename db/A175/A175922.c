/*
 * A175922  Period 5: repeat [1, 1, 2, -1, 2].
 * https://oeis.org/A175922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = [1, 1, 2, -1, 2] repeated with period 5 starting at n = 1
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Pattern of one period */
    static const seq_t pattern[5] = {1, 1, 2, (seq_t)-1, 2};

    /* The sequence is periodic: a(n) = pattern[(n-1) mod 5] */
    idx_t r = (n - 1) % 5;
    return pattern[r];
}
