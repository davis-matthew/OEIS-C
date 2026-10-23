/*
 * A143208  a(1)=2; for n>1, a(n) = (4-9*n+3*n^2)/2.
 * https://oeis.org/A143208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (4 - 9*n + 3*n^2)/2 for n>1; a(1)=2
 * Range:     i8 n <= 10, i16 n <= 149, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1) {
        return (seq_t)2;
    }
    seq_t nn = (seq_t)n * (seq_t)n;          /* n^2 */
    seq_t val = ((seq_t)3 * nn - (seq_t)9 * (seq_t)n + (seq_t)4) / 2;
    return val;
}
