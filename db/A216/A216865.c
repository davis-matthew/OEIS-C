/*
 * A216865  16k^2-32k+8 interleaved with 16k^2-16k+8 for k>=0.
 * https://oeis.org/A216865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*(2*n*(n-4) - 3*(-1)^n + 7)
 * Range:     i8 n <= 7, i16 n <= 46, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N   = n;                                /* cast index to seq_t */
    seq_t odd = (n & 1) ? (seq_t)-1 : (seq_t)1;   /* (-1)^n   */
    seq_t val = 2 * N * (N - 4) - 3 * odd + 7;    /* inside parentheses */
    return 2 * val;                              /* outer 2* */
}
