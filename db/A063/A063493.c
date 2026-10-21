/*
 * A063493  a(n) = (2*n-1)*(13*n^2-13*n+6)/6.
 * https://oeis.org/A063493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-1)*(13*n^2-13*n+6)/6
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 564, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t t1 = 2 * N - 1;                     /* 2n – 1   */
    seq_t t2 = 13 * N * N - 13 * N + 6;        /* 13n² – 13n + 6 */
    return t1 * t2 / 6;
}
