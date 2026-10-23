/*
 * A162257  a(n) = (2*n^3 + 5*n^2 - 11*n)/2.
 * https://oeis.org/A162257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (2*n^3 + 5*n^2 - 11*n)/2
 * Range:     i8 n <= 3, i16 n <= 25, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N   = n;          /* cast index to the sequence type */
    seq_t n2  = N * N;      /* n^2 */
    seq_t term1 = (seq_t)2 * n2 * N;   /* 2*n^3 */
    seq_t term2 = (seq_t)5 * n2;        /* 5*n^2 */
    seq_t term3 = (seq_t)11 * N;        /* 11*n    */
    return (term1 + term2 - term3) / 2;
}
