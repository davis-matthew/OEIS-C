/*
 * A067360  a(n) = 17^n sin(2n arctan(1/4)) or numerator of tan(2n
 *          arctan(1/4)).
 * https://oeis.org/A067360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, frac
 * Formula:   a(n) = 30*a(n-1) - 289*a(n-2), a(1)=8, a(2)=240
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 7, i64 n <= 15, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)240;
    seq_t a1 = (seq_t)8;   /* a(n-2) */
    seq_t a2 = (seq_t)240; /* a(n-1) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t ai = (seq_t)30 * a2 - (seq_t)289 * a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
