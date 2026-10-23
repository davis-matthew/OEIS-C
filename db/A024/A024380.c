/*
 * A024380  4th elementary symmetric function of the first n+3 positive
 *          integers congruent to 1 mod 4.
 * https://oeis.org/A024380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            n*(n+1)*(n+2)*(n+3)*(240*n^4+1920*n^3+4760*n^2+3072*n-1217)/360
 * Range:     u8 none, u16 none, u32 n <= 6, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    /* Compute the polynomial part */
    seq_t poly = 240 * N * N * N * N
               + 1920 * N * N * N
               + 4760 * N * N
               + 3072 * N
               - 1217;
    /* Compute the full numerator and divide by 360 */
    seq_t num = N * (N + 1) * (N + 2) * (N + 3) * poly;
    seq_t denom = (seq_t)360;
    return num / denom;
}
