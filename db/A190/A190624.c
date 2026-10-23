/*
 * A190624  Erroneous version of A190623.
 * https://oeis.org/A190624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(1)=11; for n>1, a(n)=n^3 unless n≡2 (mod 4) where a(n)=3*n^3/4
 * Range:     i8 n <= 5, i16 n <= 16, i32 n <= 16, i64 n <= 16, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)11;

    seq_t s = (seq_t)n;
    seq_t n3 = s * s  * s;          /* n^3 */
    if ((n % 4) == 2)               /* n ≡ 2 (mod 4) */
        n3 = (n3 * 3) / 4;          /* 3*n^3/4 */
    return n3;
}
