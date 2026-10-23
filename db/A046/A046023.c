/*
 * A046023  Number of ways to color edges of a tetrahedron using <= n colors.
 * https://oeis.org/A046023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6 + 3*n^4 + 8*n^2) / 12
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 60, u64 n <= 2459,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned __int128 N = n;
    unsigned __int128 n2 = N * N;          /* n^2 */
    unsigned __int128 n4 = n2 * n2;        /* n^4 */
    unsigned __int128 n6 = n4 * n2;        /* n^6 */
    unsigned __int128 r = (n6 + 3 * n4 + 8 * n2) / 12;
    return (seq_t)r;
}
