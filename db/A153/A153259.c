/*
 * A153259  a(n) = n^3 - (3*(n+3))^2.
 * https://oeis.org/A153259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n-1)^3 - 9*(n+2)^2
 * Range:     i8 n <= 1, i16 n <= 36, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* m = n-1 ;  compute as seq_t to preserve precision when seq_t is large */
    seq_t m = (seq_t)(n - 1);
    seq_t term1 = m * m * m;          /* (n-1)^3 */

    seq_t t = (seq_t)(n + 2);
    seq_t term2 = (seq_t)9 * t * t;    /* 9*(n+2)^2 */

    return term1 - term2;
}
