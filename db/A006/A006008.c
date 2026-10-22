/*
 * A006008  Number of inequivalent ways to color vertices of a regular
 *          tetrahedron using <= n colors.
 * https://oeis.org/A006008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (n^4 + 11*n^2) / 12
 * Range:     u8 n <= 7, u16 n <= 29, u32 n <= 475, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_s = (seq_t)n;
    seq_t n2 = n_s * n_s;
    return n2 * (n2 + 11) / 12;
}
