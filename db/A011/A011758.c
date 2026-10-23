/*
 * A011758  Barker sequence of length 13.
 * https://oeis.org/A011758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = the nth term of the Barker sequence of length 13; a(n)=0
 *            for n>13. The sequence is 1,1,1,1,1,-1,-1,1,1,-1,1,-1,1
 * Range:     i8 n <= 13, i16 n <= 13, i32 n <= 13, i64 n <= 13, i128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t s[13] = {
        1, 1, 1, 1, 1,
        -1, -1, 1, 1, -1,
        1, -1, 1
    };
    if (n == 0)       /* offset is 1, but guard against 0 */
        return 0;
    if (n <= 13)
        return s[n - 1];
    return 0;
}
