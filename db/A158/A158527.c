/*
 * A158527  Sum of squares of consecutive positive noncubes.
 * https://oeis.org/A158527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2)*n*(1+n)*(1+9*n+27*n^2+42*n^3+36*n^4+18*n^5+6*n^6)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* not part of the offset, safety */
    seq_t N  = (seq_t)n;                    /* promote index to sequence type */
    seq_t N2 = N * N;
    seq_t N3 = N2 * N;
    seq_t N4 = N3 * N;
    seq_t N5 = N4 * N;
    seq_t N6 = N5 * N;

    /* polynomial part */
    seq_t poly = 1
        + 9 * N
        + 27 * N2
        + 42 * N3
        + 36 * N4
        + 18 * N5
        + 6  * N6;

    /* combine everything, dividing by 2 after the n*(n+1) product */
    seq_t pair = N * (N + 1);
    pair /= 2;          /* integer division, exact */
    return pair * poly;
}
