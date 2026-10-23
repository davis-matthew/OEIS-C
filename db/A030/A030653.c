/*
 * A030653  n^3*a(n) is the number of circles in complex projective plane
 *          tangent to three smooth curves of degree n in general position.
 * https://oeis.org/A030653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = n^3 + 3*n^2 + 3*n - 3
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;            /* cast index to sequence type */
    seq_t cube = m * m * m;        /* n^3 */
    seq_t sq = 3 * m * m;          /* 3*n^2 */
    seq_t result = cube + sq + 3 * m - 3;
    return result;
}
