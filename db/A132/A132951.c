/*
 * A132951  Period 6: repeat [1, 3, 1, -1, -3, -1].
 * https://oeis.org/A132951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = [1, 3, 1, -1, -3, -1][n mod 6]
 * Range:     i8 n <= 107, i16 n <= 107, i32 n <= 107, i64 n <= 107,
 *            i128 n <= 107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic pattern of length 6 */
    static const seq_t val[6] = { (seq_t)1, (seq_t)3, (seq_t)1,
                                  (seq_t)-1, (seq_t)-3, (seq_t)-1 };
    return val[n % 6];
}
