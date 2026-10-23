/*
 * A131729  Period 4: repeat [0, 1, -1, 1].
 * https://oeis.org/A131729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(4k) = 0, a(4k+1) = 1, a(4k+2) = -1, a(4k+3) = 1 (period 4)
 * Range:     i8 n <= 113, i16 n <= 113, i32 n <= 113, i64 n <= 113,
 *            i128 n <= 113, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is unsigned; modulo 4 gives the position in the period */
    switch (n % 4U) {
        case 0: return 0;                 /* a(0), a(4), a(8), ...   */
        case 1: return 1;                 /* a(1), a(5), a(9), ...   */
        case 2: return (seq_t)-1;         /* a(2), a(6), a(10), ...  */
        default: return 1;                /* a(3), a(7), a(11), ...  */
    }
}
